# Skinny Block Cipher

Cryptography 1 (COMP.SEC.200)

## Project Members

* Mohammad Asif Ibtehaz (asif.ibtehaz@tuni.fi) (50366228)
* Henriikka Rikala (henriikka.rikala@tuni.fi) (292055)

## Code base Rule

* Use <https://github.com/shepherd-06/Skinny-Block/issues> to raise any issue, if required.

* In case of merge conflict, generate a pull request in Github and we (Ibtehaz and Henriikka) will resolve the issue together.

## Project Details

### Specification

Check: <https://csrc.nist.gov/CSRC/media/Projects/lightweight-cryptography/documents/round-2/spec-doc-rnd2/SKINNY-spec-round2.pdf>

### Other Details

* You only need to implement Section 2.3 of the specification.
* You only need to implement the SKINNY-128-384 variant.
* You only need to implement encryption, not decryption.
* Your implementation should be in the skinny function of the skinny.c file.
* You should run make test to check if your implementation is working.
* The states.txt file will tell you some intermediate state values during decryption, for the four hardcoded values in the driver_skinny.c unit test harness.

### Submission Rules

```bash
make clean

rm -f driver_skinny skinny.o

cd ..

tar czf 123456_assignment3.tar.gz assignment3

tar tzf 123456_assignment3.tar.gz
```

### How to run

```bash
make clean
make
./driver_skinny
```

or there's a `.sh` file in the root directory, give it an execute permission (+x) and then run it. That's a bash file that basically run the above command serially.

```bash
./run.sh
```

------
Kiitos, hyva paivanjatkoa! :)
