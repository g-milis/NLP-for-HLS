import torch
from transformers import AutoModel, AutoTokenizer


# Define the model repo and download model and tokenizer
model_name = "microsoft/codebert-base" 
model = AutoModel.from_pretrained(model_name)
tokenizer = AutoTokenizer.from_pretrained(model_name)

# nl_tokens = tokenizer.tokenize("Hello world!")
# code = "def max(a,b): if a>b: return a else return b"
code = "int ghhh, asdfoas, kalimera;"
code_tokens = tokenizer.tokenize(code)
# tokens = [tokenizer.cls_token] + nl_tokens + [tokenizer.sep_token] + code_tokens + [tokenizer.eos_token]
tokens = [tokenizer.cls_token] + code_tokens + [tokenizer.eos_token]
tokens_ids = tokenizer.convert_tokens_to_ids(tokens)
embeddings = model(torch.tensor(tokens_ids)[None, :])[0]

print(tokens)
print(tokens_ids)
