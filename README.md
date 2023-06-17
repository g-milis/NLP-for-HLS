# Extracting features for HLS kernels using CodeBERT

This repo uses [CodeBERT](https://github.com/microsoft/CodeBERT), a multi-programming-lingual model pre-trained on NL-PL pairs in 6 programming languages, in order to extract features from HLS kernels. The features are then visualized with dimensionality reduction methods and clustered in order to gain insights on the similarity of their execution.

### Motivation and results
See the presentation in `NLP_for_HLS.pdf`.

### Data
See `dataset` folder. 

### Dependencies
See `requirements.txt`. Optional: CUDA support for `torch`.
