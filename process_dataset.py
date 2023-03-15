import os
# Suppress KMeans memory leak
os.environ["OMP_NUM_THREADS"] = '1'

import re
import torch
import numpy as np
from glob import glob
import matplotlib.pyplot as plt
from pyparsing import nestedExpr
from os.path import join as pjoin
from sklearn.manifold import TSNE
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
from transformers import AutoModel, AutoTokenizer

from visualize import annotated_plot


# Config
path = os.path.dirname(os.path.realpath(__file__))
device = "cuda" if torch.cuda.is_available() else "cpu"
# Define the model repo and download model and tokenizer
model_name = "microsoft/codebert-base" 
model = AutoModel.from_pretrained(model_name).to(device)
tokenizer = AutoTokenizer.from_pretrained(model_name)


# Processing functions ==================================================================
def sanitize(text):
    """Remove comments and includes/pragmas."""
    return re.sub('//.*?\n|/\*.*?\*/|#.*?\n', '', text, flags=re.S)

def parse_labels(text):
    return nestedExpr('{', '}').parseString(text).asList()
    
def read_file(path):
    """Based on read_examples."""
    foldername = path.split(os.sep)[-2]
    with open(path, "r", encoding="utf-8") as f:
        # code = parse_labels(f.read())
        code = sanitize(f.read())
        code = code.replace('\n', ' ')
        code = ' '.join(code.strip().split())
    return code, foldername

# Model-specific
def tokenize_code(code, tokenizer=tokenizer):
    """Returns tokens and token_ids."""
    code_tokens = tokenizer.tokenize(code, truncation=True, max_length=510)
    tokens = [tokenizer.cls_token] + code_tokens + [tokenizer.eos_token]
    tokens_ids = tokenizer.convert_tokens_to_ids(tokens)
    return tokens, tokens_ids

def encode(token_ids, model=model):
    """Encodings have shape [batch_size, seq_length, enc_dim=768].
    We keep only the 1st element of the sequence axis, which is a general encoding."""
    encodings = model(token_ids)[0]
    return encodings[:, 0, :]


# Dataset ===============================================================================
datafiles = [
    file for file in glob(pjoin(path, 'dataset', '**', '*.c*'))
    if not file.endswith(('harness.c', 'support.c'))
]
# Read files
code_dataset = [read_file(file) for file in datafiles]
# Tokenize them and add a batch axis
data = [
    (torch.tensor(tokenize_code(code)[1], device=device)[None, :], name) 
    for code, name in code_dataset
]
# Get annotations
annotations = [name for _, name in code_dataset]

# Demo tokenization
# print(code_dataset[4][1])
# print(tokenize_code(code_dataset[4][0])[0])

# Encode dataset
encodings = []
for x, name in data:
    encodings.append(encode(x).cpu().detach().numpy())

# Create the big data matrix X
encodings = np.concatenate(encodings)



# Apply PCA
pca = PCA(3).fit_transform(encodings).astype('double')
# annotated_plot(pca, annotations, "Encodings after 3D PCA", "PCA.pdf")
# Apply t-SNE
np.random.seed(1)
tsne = TSNE(3).fit_transform(encodings).astype('double')
# annotated_plot(tsne, annotations, "Encodings after 3D t-SNE", "tSNE.pdf")


# TODO: keep L labels from code, 

# Cluster with k-means
kmeans = KMeans(n_clusters=5, n_init='auto')
labels = kmeans.fit_predict(pca)
# DecisionBoundaryDisplay.from_estimator(kmeans, pca)
# plt.show()

#Getting the Centroids
centroids = kmeans.cluster_centers_
u_labels = np.unique(labels)
 
#plotting the results:
 
for i in u_labels:
    plt.scatter(pca[labels == i , 0] , pca[labels == i , 1] , label = i)
plt.scatter(centroids[:,0] , centroids[:,1] , s = 80, color = 'k')
plt.legend()
plt.show()