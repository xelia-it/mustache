LSP Integration
===============

For LSP (Language Server protocol) integration it's useful to install _cland_.

CLangd needs a configuration file. You may add a _compile_flags.txt_ file
in the root diretory of the project.
The suggested content is:

```
-std=c++11
-fPIC
-Ithird-party/json/single_include
-Ithird-party/catch2/single_include
```
