cmake_minimum_required(VERSION 2.8)
project(drawdown CXX)
# hello.cppとgood_morning.cppをコンパイルして静的ライブラリlibgreetings.aを作成
# a.outという実行ファイルをmain.cppから作成
add_executable(drawdown.out drawdown.cpp format.cpp identifier.cpp)
# a.outを作成する際にlibgreetings.aをリンク