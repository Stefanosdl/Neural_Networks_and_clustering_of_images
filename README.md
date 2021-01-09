# Neural_Networks_and_clustering_of_images

## To run B
`make && ./search -d ../../Datasets/train-images-idx3-ubyte -i ../../NewSpace_files/output_dataset_file -q ../../Datasets/t10k-images-idx3-ubyte -s ../../NewSpace_files/output_query_file -k 4 -L 5 -o output_file`

## To run C
`make && ./search -d ../../Datasets/train-images-idx3-ubyte -q ../../Datasets/t10k-images-idx3-ubyte -l1 ../../Datasets/train-labels-idx1-ubyte -l2 ../../Datasets/t10k-labels-idx1-ubyte -o c_output_file -EMD`

## To run D
`make && ./cluster -d ../../Datasets/train-images-idx3-ubyte -i ../../NewSpace_files/output_dataset_file -n ../../Cluster_files/clusters.txt -c ../../Cluster_files/cluster.conf -o output_file`