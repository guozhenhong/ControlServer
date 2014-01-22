AUTOMAKE_OPTIONS = foreign
INCLUDES = -I$(top_srcdir)/../../lab_common/trunk/include/
bin_PROGRAMS = client
client_SOURCES = MasterAgent.cpp \
				 client.cpp \
				 MultiMsgDeserializer.cpp \
				 MultiMsgSerializer.cpp
client_LDADD = $(top_srcdir)/../../lab_common/trunk/lib/libcommon.la
