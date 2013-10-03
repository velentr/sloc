# Maintainer: Brian Kubisiak <bkubisiak.git@gmail.com>
pkgname=sloc-git
pkgver=0.2.1
pkgrel=1
pkgdesc="Simply count source lines of code"
arch=('i686' 'x86_64')
url="http://69.69.108.139/gitweb"
license=('MIT')
depends=('glibc')
makedepends=('git')
install=
source=(git+http://69.69.108.139/git/sloc.git)
md5sums=('SKIP')

_gitroot="http://69.69.108.139/git/sloc.git"
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
