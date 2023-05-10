# mmx-binaries

Binaries for the CPU / GPU plotter have moved to a new repository which supports both Chia and MMX:

https://github.com/madMAx43v3r/chia-gigahorse

## mmx-node

Binaries for `mmx-node` that support farming compressed plots, as well as regular plots.

Newer AMD GPUs starting with RX 6000 series do not work due to a driver bug, we have to wait for AMD to fix it. Vega series is known to still work when using an older driver with `amdgpu-install --opencl=pal,legacy`. RX 5000 series is unknown at this point.

### Installation

```
sudo apt install libgomp1 libsecp256k1-0 libsodium-dev libminiupnpc-dev libjemalloc-dev zlib1g-dev ocl-icd-opencl-dev clinfo
```
