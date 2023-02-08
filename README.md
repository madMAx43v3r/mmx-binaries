# mmx-binaries

Binaries for the CPU / GPU plotter have moved to a new repository which supports both Chia and MMX:

https://github.com/madMAx43v3r/chia-gigahorse

## mmx-node

Binaries for `mmx-node` that support farming compressed plots, as well as regular plots.

Supports Nvidia GPUs only at this point. Automatic fallback to CPU farming if no GPUs available.

### Installation

```
sudo apt install libgomp1 libsecp256k1-dev libsodium-dev libminiupnpc-dev libjemalloc-dev zlib1g-dev ocl-icd-opencl-dev clinfo
```
