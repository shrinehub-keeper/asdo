# Maintainer: asdo contributors
#
# This PKGBUILD is Copyright (c) 2026 asdo contributors, licensed
# under GPLv2 (or later) -- see LICENSE.GPLv2 in the asdo source tree.
# It packages the underlying OpenDoas codebase, which remains
# ISC-licensed -- see LICENSE.
pkgname=asdo
pkgver=r0.0000000
pkgrel=1
pkgdesc="A portable version of OpenBSD's doas, with sudo-style insults and a customizable prompt/error message"
arch=('x86_64' 'i686' 'aarch64' 'armv7h')
url="https://github.com/shrinehub-keeper/asdo"
license=('ISC' 'GPL2')
depends=('pam')
makedepends=('bison' 'git')
provides=('doas')
conflicts=('doas' 'opendoas')
source=("$pkgname::git+${url}.git")
sha256sums=('SKIP')

pkgver() {
	cd "$pkgname"
	git describe --long --tags 2>/dev/null | sed 's/^v//;s/\([^-]*-g\)/r\1/;s/-/./g' ||
		printf 'r%s.%s' "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
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
}
