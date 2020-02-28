#!/bin/bash -ex

export CHEERP_SRC=/tmp/compile-cheerp

mkdir -p $CHEERP_SRC
cd $CHEERP_SRC
								# LAST SEEN WORKING WITH TAG:
git clone https://github.com/leaningtech/cheerp-llvm		# 58650eeadc8b22cf81419ce08ffa1ee7ad21c292
cd  cheerp-llvm/tools/
git clone https://github.com/leaningtech/cheerp-clang clang	# 224dff3a1c14d2f5697e0ed1565ad019f8430442
cd $CHEERP_SRC
git clone https://github.com/leaningtech/cheerp-utils		# 63b8c37c5faead3483322ba321a3ee4ec7e3498f
git clone https://github.com/leaningtech/cheerp-newlib		# 875ad4f30b1ade8f005366e76cb66a7dbb411987
git clone https://github.com/leaningtech/cheerp-libcxx		# 1fadaf39e57b555c699d0deb2277e94f6594651c
git clone https://github.com/leaningtech/cheerp-libcxxabi	# 9ae50ecf5a7a4ab24113dff37a389cace8b535e1
git clone https://github.com/leaningtech/cheerp-libs		# 9eb512dd1f6c430dfdf43749dabb554fb6edabc5

cd $CHEERP_SRC/cheerp-llvm             && git checkout 58650eeadc8b22cf81419ce08ffa1ee7ad21c292
cd $CHEERP_SRC/cheerp-llvm/tools/clang && git checkout 224dff3a1c14d2f5697e0ed1565ad019f8430442
cd $CHEERP_SRC/cheerp-utils            && git checkout 63b8c37c5faead3483322ba321a3ee4ec7e3498f
cd $CHEERP_SRC/cheerp-newlib           && git checkout 875ad4f30b1ade8f005366e76cb66a7dbb411987
cd $CHEERP_SRC/cheerp-libcxx           && git checkout 1fadaf39e57b555c699d0deb2277e94f6594651c
cd $CHEERP_SRC/cheerp-libcxxabi        && git checkout 9ae50ecf5a7a4ab24113dff37a389cace8b535e1
cd $CHEERP_SRC/cheerp-libs             && git checkout 9eb512dd1f6c430dfdf43749dabb554fb6edabc5
