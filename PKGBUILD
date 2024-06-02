# Maintainer: Dima <dimaac@bk.ru>
pkgname=dimanet
pkgver=1.3
pkgrel=1
pkgdesc="DimaNet is a neural network library written in pure ANSI C."
arch=('any')
url="https://github.com/dimalmfao/dimanet"
license=('GPL-3.0')
depends=('gcc')
source=("$pkgname-$pkgver.tar.gz::https://github.com/dimalmfao/dimanet/archive/master.tar.gz")
sha256sums=('SKIP')

pkgdir=/usr/local
LIB_DIR=$pkgdir/lib
INC_DIR=$pkgdir/include

build() {
  cd "$srcdir/dimanet-master"
  make compile lib
}

package() {
  cd "$srcdir/dimanet-master"
  make install-lib DESTDIR="$pkgdir"
}
