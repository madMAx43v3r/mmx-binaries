# mmx-binaries

Binaries for the CPU / GPU plotter have moved to a new repository which supports both Chia and MMX:

https://github.com/madMAx43v3r/chia-gigahorse

## mmx-node

Binaries for `mmx-node` that support farming compressed plots, as well as regular plots.

Newer AMD GPUs starting with RX 6000 series do not work due to a driver bug, we have to wait for AMD to fix it. Vega series is known to still work when using an older driver with `amdgpu-install --opencl=pal,legacy`. RX 5000 series is unknown at this point.

### Installation

```
sudo apt install libgomp1 libsecp256k1-0 libsodium23 libminiupnpc17 libjemalloc2 zlib1g ocl-icd-libopencl1 clinfo
```
### Remote Compute Server

To offload compute from harvesters to another set of compute machines:

- Start `chia_recompute_server` on the machine that is doing the compute (available here: https://github.com/madMAx43v3r/chia-gigahorse/tree/master/chiapos).
- `export CHIAPOS_RECOMPUTE_HOST=...` on the harvester (replace `...` with the IP address or host name of the compute machine)
- On Windows you need to set `CHIAPOS_RECOMPUTE_HOST` variable via system settings.
- `CHIAPOS_RECOMPUTE_HOST` can be a list of recompute servers, such as `CHIAPOS_RECOMPUTE_HOST=192.168.0.11,192.168.0.12`. A non-standard port can be specified via `HOST:PORT` syntax, such as `localhost:12345`. Multiple servers are load balanced in a fault tolerant way.
- `CHIAPOS_RECOMPUTE_PORT` can be set to specify a custom default port for `chia_recompute_server` (default = 11989).
- See `chia_recompute_server --help` for available options.

![Remote_Compute_Drawings drawio](https://github.com/madMAx43v3r/chia-gigahorse/assets/951738/9bb8d9b7-6a15-4b4a-82aa-6ab72471d5e5)

### Remote Compute Proxy

To distribute farming compute to multiple compute machines in a centralized manner:

- Start `chia_recompute_proxy -n B -n C ...` on a machine `A`. (`B`, `C`, etc are running `chia_recompute_server`)
- Set `CHIAPOS_RECOMPUTE_HOST` on your harvester(s) to machine A.
- `chia_recompute_proxy` can be run on a central machine, or on each harvester itself, in which case `A = localhost`.
- See `chia_recompute_proxy --help` for available options.

