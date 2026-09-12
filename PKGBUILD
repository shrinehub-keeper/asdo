# Maintainer: asdo contributors
#
# This PKGBUILD is Copyright (c) 2026 asdo contributors, licensed
# under GPLv2 (only, not 'or later') -- see LICENSE.GPLv2 in the asdo source tree.
# It packages the underlying OpenDoas codebase, which remains
# ISC-licensed -- see LICENSE.
pkgname=asdo
pkgver=0.r294.3e57786
pkgrel=1
pkgdesc="A portable version of OpenBSD's doas, with sudo-style insults and a customizable prompt/error message"
arch=('x86_64' 'i686' 'aarch64' 'armv7h')
url="https://github.com/shrinehub-keeper/asdo"
license=('ISC' 'GPL2')
depends=('pam')
optdepends=(
	'doas-sudo-shim: (from AUR, only place where it is) for compatibility with sudo'
)
makedepends=('bison' 'git')
provides=('doas')
conflicts=('doas' 'opendoas' 'sudo')
source=("$pkgname::git+${url}.git")
sha256sums=('SKIP')
backup=('etc/doas.conf')

pkgver() {
  cd "$srcdir/$pkgname"
  if git describe --long --tags >/dev/null 2>&1; then
    git describe --long --tags \
      | sed 's/^v//;s/\([^-]*-g\)/r\1/;s/-/./g'
  else
    printf "0.r%s.%s" \
      "$(git rev-list --count HEAD)" \
      "$(git rev-parse --short HEAD)"
  fi
}

build() {
	cd "$pkgname"
	./configure \
		--prefix=/usr \
		--sysconfdir=/etc \
		--with-pam \
		--with-timestamp
	make
}

package() {
	cd "$pkgname"
	make DESTDIR="$pkgdir" install

	# The build produces a binary named "doas" (upstream OpenDoas's PROG).
	# asdo ships that binary as /usr/bin/asdo, and installs
	# /usr/bin/doas as a doas-compatible symlink to it, so anything
	# that calls "doas" keeps working unmodified.
	mv "$pkgdir/usr/bin/doas" "$pkgdir/usr/bin/asdo"
	ln -s asdo "$pkgdir/usr/bin/doas"

	install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
	install -Dm644 LICENSE.GPLv2 "$pkgdir/usr/share/licenses/$pkgname/LICENSE.GPLv2"
	install -Dm644 CONFIGURING.md "$pkgdir/usr/share/doc/$pkgname/CONFIGURING.md"
	install -Dm600 doas.conf "$pkgdir/etc/doas.conf"
	install -Dm600 doas.conf "$pkgdir/etc/asdo.conf"
}
