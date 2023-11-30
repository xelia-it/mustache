Versioning
==========

Starting from commit _dfdef8f_ we have changed the versioning method.

We now use _release_ branches for distributing production-ready code.

We follow _semantic versioning_.
Each release has a tag `relM.m.p` where `M` is the major version,
`m` is the minor version and `p` is the minor.

A change in the _major_ version means that _breaking_ changes has
been done in the interfaces and/or ABI so the code is not backward
compatible.

A change in the _minor_ version means that _new features_ has been
added but each is backward compatible.

A _patch_ name change means that only bugfixes has been added.
