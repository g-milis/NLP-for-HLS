""" Reads the data, extracts model features and visualizes the results. """
import os
# Fix for KMeans memory leak on Windows
os.environ["OMP_NUM_THREADS"] = '1'

import torch
import numpy as np
from glob import glob
from os.path import join as pjoin
from sklearn.manifold import TSNE
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
from transformers import AutoModel, AutoTokenizer

import data
import visualize


# Select whether to process dataset by keeping code labels or
# read the whole files and let the tokenizer truncate them
PROCESS = False

# Utilities
note = '_original' if not PROCESS else ''
path = os.path.dirname(os.path.realpath(__file__))
device = "cuda" if torch.cuda.is_available() else "cpu"

# Define the model repo and download model and tokenizer
model_name = "microsoft/codebert-base"
tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModel.from_pretrained(model_name).to(device)


# Model utility functions ===============================================================
def tokenize_code(code, tokenizer=tokenizer):
    """ Returns tokens and token_ids. """
    code_tokens = tokenizer.tokenize(code, truncation=True, max_length=510)
    tokens = [tokenizer.cls_token] + code_tokens + [tokenizer.eos_token]
    tokens_ids = tokenizer.convert_tokens_to_ids(tokens)
    return tokens, tokens_ids

def encode(token_ids, model=model):
    """ Encodings have shape [batch_size, seq_length, enc_dim=768]. We keep only
    the 1st element of the sequence axis, which is the classification encoding. """
    encodings = model(token_ids)[0]
    return encodings[:, 0, :]


# Dataset ===============================================================================
if PROCESS:
    datafiles = [
        file for file in glob(pjoin(path, 'dataset', '*', 'processed.cpp'))
        if not file.endswith(('harness.c', 'support.c'))
    ]
else:
    datafiles = [
        file for file in glob(pjoin(path, 'dataset', '*', '*.c*'))
        if not file.endswith(('harness.c', 'support.c', 'processed.cpp'))
    ]

# Read files
code_dataset = [data.read_file(file) for file in datafiles]

# Check that we don't have any empty strings in the dataset
assert not [data for data in code_dataset if data[0] == '']

# Tokenize them and add a batch axis
data = [
    (torch.tensor(tokenize_code(code)[1], device=device)[None, :], name) 
    for code, name in code_dataset
]
# Get annotations
annotations = [name for _, name in code_dataset]


# Encode dataset ========================================================================
encodings = []
for x, name in data:
    encodings.append(encode(x).cpu().detach().numpy())

# Create the big data matrix X
encodings = np.concatenate(encodings)


# Visualize =============================================================================
def cluster(k, data):
    """ Applies k-means clustering, returns labels and cetroids. """
    kmeans = KMeans(n_clusters=k, n_init='auto')
    labels = kmeans.fit_predict(data)
    centroids = kmeans.cluster_centers_
    return labels, centroids

def group_annotations(labels, annotations):
    """ Groups the names of each benchmark according to clustering labels. """
    u_labels = np.unique(labels)
    groups = []
    for label in u_labels:
        group = sorted([text for i, text in enumerate(annotations) if labels[i] == label])
        groups.append(group)
        print(f"Cluster {label + 1}:")
        print(*group, sep='\n')
    return groups


# Apply PCA
pca = PCA(3).fit_transform(encodings).astype('double')
visualize.interactive_svg(
    pca,
    annotations,
    "Encodings after 3D PCA",
    pjoin(path, 'images', f"PCA{note}")
)

# Apply t-SNE
np.random.seed(100)
tsne = TSNE(3).fit_transform(encodings).astype('double')
visualize.interactive_svg(
    tsne,
    annotations,
    "Encodings after 3D t-SNE",
    pjoin(path, 'images', f"tSNE{note}")
)

# Cluster with k-means
for k in range(4, 7):
    print(f"{k}-means clustering:")
    labels, centroids = cluster(k, pca)
    visualize.k_means_plot(
        pca,
        labels,
        centroids, 
        annotations,
        f"{k}-means clustering of 3D PCA",
        pjoin(path, 'images', f"PCA_{k}{note}.pdf")
    )
    print("PCA:")
    group_annotations(labels, annotations)
    print()

    labels, centroids = cluster(k, tsne)
    visualize.k_means_plot(
        tsne,
        labels,
        centroids, 
        annotations,
        f"{k}-means clustering of 3D t-SNE",
        pjoin(path, 'images', f"tSNE_{k}{note}.pdf")
    )
    print("t-SNE:")
    group_annotations(labels, annotations)
    print()
