augroup project
	autocmd!
	autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END
"let &path.="/Library/Frameworks/SDL2.framework/Headers/,"
"let &path.="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1,"
"let &path.="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/8.0.0/include,"
"let &path.="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include,"
"let &path.="/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/usr/include,"
"let &path.=""
let &path.="/usr/local/Cellar/gcc-arm-none-eabi/20171218/lib/gcc/arm-none-eabi/7.2.1/include/,"
" Custom build options
" set makeprg=make\ -C\ ../build\ -j9

