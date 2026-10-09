pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = gaiksaurus-1.0.pc

hicolor16dir = $(datadir)/icons/hicolor/16x16/apps
hicolor16_DATA = icons/16x16/gaiksaurus.png
hicolor22dir = $(datadir)/icons/hicolor/22x22/apps
hicolor22_DATA = icons/22x22/gaiksaurus.png
hicolor24dir = $(datadir)/icons/hicolor/24x24/apps
hicolor24_DATA = icons/24x24/gaiksaurus.png
hicolor32dir = $(datadir)/icons/hicolor/32x32/apps
hicolor32_DATA = icons/32x32/gaiksaurus.png
hicolor48dir = $(datadir)/icons/hicolor/48x48/apps
hicolor48_DATA = icons/48x48/gaiksaurus.png
hicolor64dir = $(datadir)/icons/hicolor/64x64/apps
hicolor64_DATA = icons/64x64/gaiksaurus.png
hicolor128dir = $(datadir)/icons/hicolor/128x128/apps
hicolor128_DATA = icons/128x128/gaiksaurus.png
hicolor256dir = $(datadir)/icons/hicolor/256x256/apps
hicolor256_DATA = icons/256x256/gaiksaurus.png
hicolor512dir = $(datadir)/icons/hicolor/512x512/apps
hicolor512_DATA = icons/512x512/gaiksaurus.png

man_MANS = gaiksaurus.1

applicationsdir = $(datadir)/applications
applications_DATA = gaiksaurus.desktop

SUBDIRS = src 

EXTRA_DIST = gaiksaurus-1.0.pc.in icons/16x16/gaiksaurus.png icons/22x22/gaiksaurus.png icons/24x24/gaiksaurus.png icons/32x32/gaiksaurus.png icons/48x48/gaiksaurus.png icons/64x64/gaiksaurus.png icons/128x128/gaiksaurus.png icons/256x256/gaiksaurus.png icons/512x512/gaiksaurus.png gaiksaurus.desktop gaiksaurus.1

# refresh the icon cache (not when staging into a DESTDIR; package
# managers should do that after installing)
install-data-hook:
	@if test -z "$(DESTDIR)"; then \
	  gtk-update-icon-cache -f -t $(datadir)/icons/hicolor >/dev/null 2>&1 || :; \
	fi
