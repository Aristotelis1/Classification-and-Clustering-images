# Classification-and-Clustering-images

README pdf is currently available only in Greek.

## Summary ##

# Metric: 
Manhattan distance was used to calculate the distance between images.

# Locality Sensitive Hashing:
Idea: use hash-table for proximity query, mapping nearby vectors to same
bucket

LSH creates hash-tables using amplified index function g by combining k
functions hi ∈R H, chosen uniformly at random with repetition from H.

-Two different methods were implemented for searching similar images, approximate KNN and Range Search.

# Binary Hypercube
Let fi s.t: for each hi, fi(hi) maps buckets to {0, 1} uniformly
The idea is to check images in same vertex and nearby vertices in increasing Hamming
distance (=1, then 2, etc), until some threshold reached.

# Clustering
K-means++ approach was implemented for the initialization part of the centroids and then Lloyd's algorithm.

# Evaluation
Silhouette function is used to evaluate the clustering
s(i)e[-1,1]
s(i) -> 1: i seems correctly assigned to its cluster;
s(i) -> 0: borderline assignment (but not worth to change);
s(i) -> −1: i would be better if assigned to next best cluster.

