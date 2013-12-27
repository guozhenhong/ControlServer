INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/../lab_common/trunk/include
bin_PROGRAMS = $(top_srcdir)/bin/server
__top_srcdir__bin_server_SOURCES = ControlAgent.cpp \
				 					RegisterServerInfoTask.cpp \
				 					RegisterWorkItem.cpp \
				 					MultiMsgDeserializer.cpp \
				 					MultiMsgSerializer.cpp \
									GetServerInfoTask.cpp \
									GetServerInfoWorkItem.cpp \
				 					server.cpp
__top_srcdir__bin_server_LDADD = $(top_srcdir)/../lab_common/trunk/lib/libcommon.la \
			   						../controlDB/libdb.la
