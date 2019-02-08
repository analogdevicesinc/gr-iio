#!/bin/sh
set -e

NUM_JOBS=3

__cmake() {
	local args="$1"
	mkdir -p build
	cd build # build

	if [ "$TRAVIS" = "true" ] || [ "$INSIDE_DOCKER" = "1" ] ; then
		cmake $args ..
		make -j${NUM_JOBS}
		sudo make install
	else
		cmake -DCMAKE_PREFIX_PATH="$STAGINGDIR" -DCMAKE_INSTALL_PREFIX="$STAGINGDIR" \
			-DCMAKE_EXE_LINKER_FLAGS="-L${STAGINGDIR}/lib" \
			$args .. $SILENCED
		CFLAGS=-I${STAGINGDIR}/include LDFLAGS=-L${STAGINGDIR}/lib make -j${NUM_JOBS} $SILENCED
		make install
	fi

	cd ..
}

__make() {
	$preconfigure
	if [ "$TRAVIS" = "true" ] || [ "$INSIDE_DOCKER" = "1" ] ; then
		$configure
		$make -j${NUM_JOBS}
		sudo $make install
	else
		$configure --prefix="$STAGINGDIR" $SILENCED
		CFLAGS=-I${STAGINGDIR}/include LDFLAGS=-L${STAGINGDIR}/lib $make -j${NUM_JOBS} $SILENCED
		$SUDO $make install
	fi
}

__build_common() {
	local dir="$1"
	local buildfunc="$2"
	local getfunc="$3"
	local subdir="$4"
	local args="$5"

	cd "$WORKDIR" # deps dir

	# if we have this folder, we may not need to download it
	[ -d "$dir" ] || $getfunc

	cd "$dir" # this dep dir
	[ -z "$subdir" ] || cd "$subdir" # in case there is a build subdir or smth

	$buildfunc "$args"

	cd ..
	cd ..
	[ -z "$subdir" ] || cd ..
}

wget_and_untar() {
	[ -d "$WORKDIR/$dir" ] || {
		local tar_file="${dir}.tar.gz"
		wget --no-check-certificate "$url" -O "$tar_file"
		tar -xvf "$tar_file" > /dev/null
		[ -z "$patchfunc" ] || {
			cd $dir
			$patchfunc
			cd ..
		}
	}
}

git_clone_update() {
	[ -d "$WORKDIR/$dir" ] || {
		[ -z "$branch" ] || branch="-b $branch"
		git clone --recursive $branch "$url" "$dir"
		[ -z "$patchfunc" ] || {
			cd $dir
			$patchfunc
			cd ..
		}
	}
}

cmake_build_wget() {
	local dir="$1"
	local url="$2"

	__build_common "$dir" "__cmake" "wget_and_untar"
}

cmake_build_git() {
	local dir="$1"
	local url="$2"
	local branch="$3"
	local args="$4"

	__build_common "$dir" "__cmake" "git_clone_update" "" "$args"
}

make_build_wget() {
	local dir="$1"
	local url="$2"
	local configure="${3:-./configure}"
	local make="${4:-make}"

	__build_common "$dir" "__make" "wget_and_untar"
}

make_build_git() {
	local dir="$1"
	local url="$2"
	local configure="${3:-./configure}"
	local make="${4:-make}"
	local preconfigure="$5"

	__build_common "$dir" "__make" "git_clone_update"
}


__handle_common() {
	# -DENABLE_INTERNAL_VOLK:BOOL=OFF
	# FIXME: see about keeping a deb/rpm somewhere cached for this
	cmake_build_git "gnuradio" \
		"https://github.com/analogdevicesinc/gnuradio" \
		"signal_source_phase_rebased" \
		"-DENABLE_INTERNAL_VOLK:BOOL=ON -DENABLE_GR_FEC:BOOL=OFF -DENABLE_GR_DIGITAL:BOOL=OFF -DENABLE_GR_DTV:BOOL=OFF -DENABLE_GR_ATSC:BOOL=OFF -DENABLE_GR_AUDIO:BOOL=OFF -DENABLE_GR_CHANNELS:BOOL=OFF -DENABLE_GR_NOAA:BOOL=OFF -DENABLE_GR_PAGER:BOOL=OFF -DENABLE_GR_TRELLIS:BOOL=OFF -DENABLE_GR_VOCODER:BOOL=OFF"
}

