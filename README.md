# ForceD3

> Cloned from https://github.com/leiless/iokit_kext

This kext forces D3 (sleep) slot to specified PCI slots. This isn't well tested, but it works well for me when disabling NVMe slot and dGPU.

## Usage

- Modify `IOKitPersonalities` section of `Info.plist` to your desired condition. On my settings I've set that to match my `RP05` and `RP05` PCIe slot. You can see what `IONameMatch` to use with IORegisteryExplorer or Hackintool.
- Inject with Opencore or Clover.

