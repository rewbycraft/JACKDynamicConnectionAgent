# Maintainer: rewbycraft <contact at roelf dot org

pkgname=jackdynconagent-git
_pkgname=jackdynconagent
pkgver=0.1
pkgrel=1
pkgdesc="The purpose of this program is to dynamically match JACK clients and remap their ports as required."
arch=('i686' 'x86_64')
url="https://github.com/rewbycraft/JACKDynamicConnectionAgent"
license=('MIT')
depends=('jack2' 'yaml-cpp' 'procps-ng')
makedepends=('cmake' 'git')
provides=('jackdynconagent')
conflicts=('jackdynconagent')
source=("$_pkgname::git://github.com/rewbycraft/JACKDynamicConnectionAgent.git#branch=master")
sha256sums=('SKIP')

pkgver() {
  cd "$srcdir/$_pkgname"
  git describe --long --tags | sed -r 's/([^-]*-g)/r\1/;s/-/./g'
}

build() {
  cd "$srcdir/$_pkgname"
  cmake -DCMAKE_INSTALL_PREFIX=/usr
  make
}

package() {
  cd "$srcdir/$_pkgname"
  make DESTDIR="$pkgdir" install
}

