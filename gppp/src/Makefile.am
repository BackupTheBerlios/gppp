INCLUDES = -I$(top_srcdir) -I$(includedir) $(GNOME_INCLUDEDIR) \
         -DG_LOG_DOMAIN=\"gppp\" -DGNOMELOCALEDIR=\""$(datadir)/locale"\" \
	-I../intl -I$(top_srcdir)/intl

bin_PROGRAMS = gppp

gppp_SOURCES =  storeconfig.c\
				accounts.c\
				stuff.c\
				setup.c\
				gppp_main.c\
				gppp.c\
				storeconfig.h\
				accounts.h\
				stuff.h\
				setup.h\
				gppp_main.h\
				gppp.h
   

gppp_LDADD = $(GNOMEUI_LIBS) $(GNOME_LIBDIR) $(INTLLIBS)
