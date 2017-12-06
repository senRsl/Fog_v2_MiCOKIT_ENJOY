############################################################################### 
#
#  The MIT License
#  Copyright (c) 2016 MXCHIP Inc.
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy 
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights 
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is furnished
#  to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
#  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
#  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
###############################################################################

NAME := Lib_fog_v2


$(NAME)_SOURCES :=	fog_bonjour/fog_bonjour.c \
					fog_http/fog_http_client.c \
					fog_http/fog_v2_sub.c \
					fog_http/fog_v2_sub_device_management.c \
					fog_http/fog_v2.c \
					fog_http/fog_v2_user_notification.c \
					fog_mqtt/fog_mqtt.c \
					fog_mqtt/fog_process_mqtt_cmd.c \
					fog_ota/fog_ota.c \
					fog_ota/fog2_ota_notification.c \
					fog_ota/url.c \
					fog_tcp_server/fog_tcp_server.c
					
										
GLOBAL_INCLUDES += 	.

$(NAME)_COMPONENTS := FOG_V2/lib/mqtt	#MICOSDK_#3.2
	