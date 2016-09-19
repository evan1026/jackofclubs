libs="cmake git libfreetype6-dev libjpeg-dev libx11-dev libxrandr-dev libxcb-present-dev libx11-xcb-dev libxcb-randr0-dev libxcb-image0-dev libgl1-mesa-dev libflac-dev libogg-dev libvorbis-dev libvorbisenc2 libvorbisfile3 libopenal-dev libpthread-stubs0-dev"
sudo apt-get install $libs

tmpdir=$(mktemp -d)
cd $tmpdir

git clone https://github.com/SFML/SFML.git
cd SFML

mkdir build
cd build

cmake ..
make
sudo make install

cd /
rm -rf $tmpdir
