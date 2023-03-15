import os
import json
import torch
from os.path import join as pjoin
from transformers import AutoModel, AutoTokenizer
from torch.utils.data import DataLoader, SequentialSampler, TensorDataset

from CodeBERT.code2nl import bleu


# Define the model repo and download model and tokenizer
model_name = "microsoft/codebert-base" 
model = AutoModel.from_pretrained(model_name)
tokenizer = AutoTokenizer.from_pretrained(model_name)


class Example(object):
    """A single training/test example."""
    def __init__(self, idx, source, target):
        self.idx = idx
        self.source = source
        self.target = target

class InputFeatures(object):
    """A single training/test features for a example."""
    def __init__(self, example_id, source_ids, target_ids, source_mask, target_mask):
        self.example_id = example_id
        self.source_ids = source_ids
        self.target_ids = target_ids
        self.source_mask = source_mask
        self.target_mask = target_mask     

def read_examples(filename):
    """Read examples from filename."""
    examples = []
    with open(filename, encoding="utf-8") as f:
        for idx, line in enumerate(f):
            line = line.strip()
            js = json.loads(line)
            if 'idx' not in js:
                js['idx'] = idx
            code = ' '.join(js['code_tokens']).replace('\n', ' ')
            code = ' '.join(code.strip().split())
            nl = ' '.join(js['docstring_tokens']).replace('\n', '')
            nl = ' '.join(nl.strip().split())            
            examples.append(Example(idx, code, nl))
    return examples


def read_file(path):
    """Based on read_examples."""
    with open(path, "r", encoding="utf-8") as f:
        code = f.read().replace('\n', ' ')
        code = ' '.join(code.strip().split())
    return code
  


def convert_examples_to_features(examples, tokenizer, args, stage=None):
    features = []
    for example_index, example in enumerate(examples):
        # source
        source_tokens = tokenizer.tokenize(example.source)[:args.max_source_length-2]
        source_tokens = [tokenizer.cls_token] + source_tokens + [tokenizer.sep_token]
        source_ids = tokenizer.convert_tokens_to_ids(source_tokens) 
        source_mask = [1] * (len(source_tokens))
        padding_length = args.max_source_length - len(source_ids)
        source_ids += [tokenizer.pad_token_id]*padding_length
        source_mask += [0]*padding_length
 
        # target
        if stage=="test":
            target_tokens = tokenizer.tokenize("None")
        else:
            target_tokens = tokenizer.tokenize(example.target)[:args.max_target_length-2]
        target_tokens = [tokenizer.cls_token]+target_tokens+[tokenizer.sep_token]            
        target_ids = tokenizer.convert_tokens_to_ids(target_tokens)
        target_mask = [1] *len(target_ids)
        padding_length = args.max_target_length - len(target_ids)
        target_ids += [tokenizer.pad_token_id]*padding_length
        target_mask += [0]*padding_length   
       
        features.append(
            InputFeatures(
                example_index,
                source_ids,
                target_ids,
                source_mask,
                target_mask,
            )
        )
    return features

device = "cuda" if torch.cuda.is_available() else "cpu"
path = os.path.dirname(os.path.realpath(__file__))


# Transform input tokens 
in1 = "def add(a, b): return a+b"
in2 = "int add(int a, int b){ return a+b; }"


def encode(code, tokenizer=tokenizer, model=model, pool='mean'):
    sequence = tokenizer(code, return_tensors="pt")
    print(tokenizer.tokenize(code))
    encodings = model(**sequence)[0]
    if pool == 'mean':
        return encodings.mean(dim=1)
    if pool == 'max':
        # torch.return_types.max is a tuple containing the indices, too
        return encodings.max(dim=1)[0]


def encode_text(code, tokenizer=tokenizer, model=model):
    code_tokens = tokenizer.tokenize(code)
    tokens = [tokenizer.cls_token] + code_tokens + [tokenizer.eos_token]
    tokens_ids = tokenizer.convert_tokens_to_ids(tokens)
    encodings = model(torch.tensor(tokens_ids)[None, :])[0]
    return encodings[:, 0, :]


# Apply model
out1 = encode_text(in1)
out2 = encode_text(in2)

# Find distances from encoding vectors
def distance(enc1, enc2):
    return round((enc1 - enc2).pow(2).sum().sqrt().item(), 5)


def distance(enc1, enc2):
    return round(torch.nn.CosineSimilarity()(enc1, enc2)[0].item(), 5)


print('add:', distance(out1, out2))


file_cpp1 = pjoin(path, 'tests', 'calcDist.cpp')
file_cpp2 = pjoin(path, 'tests', 'unopt.cpp')
file_py1 = pjoin(path, 'tests', 'lib.py')
file_py2 = pjoin(path, 'tests', 'plot.py')

code_cpp1 = read_file(file_cpp1)
enc_cpp1 = encode_text(code_cpp1)

code_cpp2 = read_file(file_cpp2)
enc_cpp2 = encode_text(code_cpp2)

code_py1 = read_file(file_py1)
enc_py1 = encode_text(code_py1)

code_py2 = read_file(file_py2)
enc_py2 = encode_text(code_py2)


print("C++:", distance(enc_cpp1, enc_cpp2))
print("Python:", distance(enc_py1, enc_py2))
print("Between:", distance(enc_cpp1, enc_py1))
print("Between:", distance(enc_cpp1, enc_py2))
print("Between:", distance(enc_cpp2, enc_py1))
print("Between:", distance(enc_cpp2, enc_py2))

exit()
files = []
args = 0

for idx, file in enumerate(files):   
    eval_examples = read_examples(file)
    eval_features = convert_examples_to_features(eval_examples, tokenizer, args, stage='test')
    all_source_ids = torch.tensor([f.source_ids for f in eval_features], dtype=torch.long)
    all_source_mask = torch.tensor([f.source_mask for f in eval_features], dtype=torch.long)    
    eval_data = TensorDataset(all_source_ids, all_source_mask)   

    # Calculate bleu
    eval_sampler = SequentialSampler(eval_data)
    eval_dataloader = DataLoader(eval_data, sampler=eval_sampler, batch_size=args.eval_batch_size)

    model.eval() 
    p = []
    for batch in eval_dataloader:
        batch = tuple(t.to(device) for t in batch)
        source_ids, source_mask = batch                  
        with torch.no_grad():
            preds = model(source_ids=source_ids, source_mask=source_mask)  
            for pred in preds:
                t = pred[0].cpu().numpy()
                t = list(t)
                if 0 in t:
                    t=t[:t.index(0)]
                text = tokenizer.decode(t, clean_up_tokenization_spaces=False)
                p.append(text)
    model.train()
    predictions = []
    with open(os.path.join(args.output_dir, f"test_{idx}.output"), 'w') as f, open(os.path.join(args.output_dir, f"test_{idx}.gold"), 'w') as f1:
        for ref, gold in zip(p,eval_examples):
            predictions.append(str(gold.idx) + '\t' + ref)
            f.write(str(gold.idx) + '\t' + ref + '\n')
            f1.write(str(gold.idx) + '\t' + gold.target + '\n')     

    goldMap, predictionMap = bleu.computeMaps(predictions, os.path.join(args.output_dir, f"test_{idx}.gold")) 
    dev_bleu = round(bleu.bleuFromMaps(goldMap, predictionMap)[0], 2)
