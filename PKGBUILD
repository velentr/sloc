# Maintainer: Brian Kubisiak <velentr.rc@gmail.com>
pkgname=sloc
pkgver=1.1
pkgrel=1
pkgdesc="Simply count source lines of code"
arch=('i686' 'x86_64')
url="https://github.com/velentr/sloc.git"
license=('MIT')
depends=('glibc')
makedepends=('git')
install=
source=(git+https://github.com/velentr/sloc.git)
md5sums=('SKIP')

_gitroot="https://github.com/velentr/sloc.git"
_gitname="sloc"

build() {
  cd "$srcdir"
  msg "Connecting to GIT server...."

  if [[ -d "$_gitname" ]]; then
    cd "$_gitname" && git pull origin
    msg "The local files are updated."
  else
    git clone "$_gitroot" "$_gitname"
  fi

  msg "GIT checkout done or server timeout"
  msg "Starting build..."

  rm -rf "$srcdir/$_gitname-build"
  git clone "$srcdir/$_gitname" "$srcdir/$_gitname-build"
  cd "$srcdir/$_gitname-build"

  make
}

package() {
  cd "$srcdir/$_gitname-build"
  make PREFIX=/usr DESTDIR="$pkgdir/" install
  install -m644 -D LICENSE $pkgdir/usr/share/licenses/$pkgname/LICENSE
}

# vim:set ts=2 sw=2 et:
