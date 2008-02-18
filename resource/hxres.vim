" Vim syntax file
" Language:	Hoe resource files
" Maintainer: Hoe devel group

" Don't forget to run your config file through testparm(1)!

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

"syn case ignore

syn match sambaParameter /^[a-zA-Z \t]\+=/ contains=sambaKeyword
syn match sambaSection /^\s*\[[a-zA-Z0-9_\-.$ ]\+\]/
syn match sambaMacro /%[SPugUGHvhmLMNpRdaITD]/
syn match sambaMacro /%$([a-zA-Z0-9_]\+)/
syn match hoeComment /\/\/.*/
syn match hoeString /"[^"]*"/
syn match sambaContinue /\\$/
syn keyword hoeBoolean true false yes no

" Keywords
syn keyword hoeType Namespace Picture

syn keyword hoeKeyword contained Namespace Picture acl action add address admin aliases
syn keyword hoeKeyword contained store unknown unwriteable

" Define the default highlighting.
" For version 5.7 and earlier: only when not done already
" For version 5.8 and later: only when an item doesn't have highlighting yet
if version >= 508
  if version < 508
    let did_samba_syn_inits = 1
    command -nargs=+ HiLink hi link <args>
  else
    command -nargs=+ HiLink hi def link <args>
  endif
  HiLink sambaParameter Normal
  HiLink hoeKeyword     Type
  HiLink hoeType        Type
  HiLink sambaSection   Statement
  HiLink sambaMacro     PreProc
  HiLink hoeComment     Comment
  HiLink sambaContinue  Operator
  HiLink hoeBoolean     Constant
  HiLink hoeString      String
  delcommand HiLink
endif

let b:current_syntax = "hxres"

" vim: ts=8
