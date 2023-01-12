# MMX chiapos

Use `-r 8` to get optimal performance, this will run `check` and `lookup` in parallel using 8 threads.

In case of CPU only and more than 8 cores, you can increase `-r` to get more performace.

The harvester and this tool will automatically use all availalable GPUs where it is supported.
Lower C levels will use CPU only since GPU is not efficient for those.

To disable GPU usage or to limit to a certain number of GPUs you can set a environment variable:
```
export CHIAPOS_MAX_CUDA_DEVICES=0
```
For example if you have a fast and a slow GPU, set `CHIAPOS_MAX_CUDA_DEVICES=1`, to only use the fast one.

## Checking plots

./ProofOfSpace check -r 8 -f *.plot

## Farming benchmark

To check how many plots of a certain K size and C level you can farm on a machine:

```
time ./ProofOfSpace lookup -r 8 -f *.plot
```

This will measure the total time of performing 1000 lookups.

To convert the total time to maximum farm size:
```
max_farm_size = plot_size * plot_filter * 8 * 1000 / total_lookup_time_seconds;
```
The plot filter on MMX testnet10 and mainnet will be `256`, while Chia and testnet9 are using `512`.

In case of CPU farming, set `-r` to the number of CPU cores / threads.

