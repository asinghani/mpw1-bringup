# mpw1-bringup

Experiments in salvating SKY130 MPW-ONE chips.

### Background

MPW-ONE was the first round of the open-source Google-sponsored tapeout, and as a result of tooling issues, the standard "caravel" padframe used for all chips on the MPW contains timing violations). The design I am working with is my submission to MPW-ONE - [asinghani/crypto-accelerator-chip](https://github.com/asinghani/crypto-accelerator-chip)

Early experiments by efabless and [the PyFive-RISC-V team](https://github.com/PyFive-RISC-V/pyfive-mpw1-postmortem) reveal that some limited functionality may be accessible through precise voltage control.

### Plan

- [ ] Fabricate test boards
- [ ] Write test RISC-V code
- [ ] Attempt to bring up caravel using precise bench power supplies
- [ ] Write scripts to automate testing/characterization of working/nonworking chips and (if possible) stress-test them. Potentially experiment with external thermal coersion to improve timing.
- [ ] Amongst working chips, attempt to find chips with the most reliability / largest functional range
- [ ] Attempt to bring up crypto-accelerator core and run functionality tests
- [ ] (Unlikely, requires high clock speeds and long, reliable runs) If limited I/O is configurable, attempt to get VGA video output - potentially running at a fractional clock speed and using an FPGA to resample the video to a higher framerate
