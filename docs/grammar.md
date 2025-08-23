# Grammar

## Tokens

```
  sv  = start variable = {{
  svu = start variable unescaped= {{{
  sc  = start comment = {{!
  sb  = start begin of section = {{#
  se  = start end of section = {{/
  si  = start if = {{=
  s0  = start existing test = {{0
  su  = start unless = {{^
  sp  = start partial = {{>
  st  = start template = {{<
  ee  = end = }}
  eeu = end = }}}
  txt = (sequence of txt)
```

## Grammar

```
  MESSAGE            := VARIABLE MESSAGE | VARIABLE_UNESCAPED MESSAGE |
                        COMMENT MESSAGE | SECTION MESSAGE |
                        IF MESSAGE | UNLESS MESSAGE | EXISTS_TEST MESSAGE |
                        PARTIAL MESSAGE | txt MESSAGE | (empty)
  VARIABLE           := sv  txt ee
  VARIABLE_UNESCAPED := svu txt eeu
  COMMENT            := sc  txt ee
  IF                 := si  txt ee
  EXISTS_TEST        := s0  txt ee
  UNLESS             := sv  txt ee
  SECTION            := sb  txt ee MESSAGE se txt ee | sbi txt ee MESSAGE se txt ee
  PARTIAL            := sp  txt ee | st txt ee
```
