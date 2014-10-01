#! /usr/bin/env python
import requests
from requests.auth import HTTPDigestAuth
import sys

FRITZ_IP_ADDRESS = '192.168.178.1'
FRITZ_TCP_PORT = 49000
# digest authentication
FRITZ_USERNAME = 'dslf-config'
FRITZ_REALM = 'F!Box SOAP-Auth'

#CONTROL_URL = 'upnp/control/lanconfigsecurity'

# mit ner '1' hinten raus ist das 1. WLAN Gerät gemeint, also 2.4 GHz
# mit '2' hinten raus (auch bei der SOAPACTION und im Body beachten!) ist 
# das 5 GHz device gemeint, siehe 'wlanconfigSCPD.pdf'
CONTROL_URL = 'upnp/control/wlanconfig1'

#CONTROL_URL2 = 'upnp/control/deviceinfo'

#SOAPACTION = 'urn:dslforum-org:service:DeviceInfo:1#GetSecurityPort'
#SOAPACTION = 'urn:dslforum-org:service:LANConfigSecurity:1#GetInfo'
# serviceType#Action
SOAPACTION = 'urn:dslforum-org:service:WLANConfiguration:1#SetChannel'
#SOAPACTION2 = 'urn:dslforum-org:service:DeviceInfo:1#GetDeviceLog'

header = {'soapaction': SOAPACTION,
          'content-type': 'text/xml',
          'charset': 'utf-8'}

body = """
    <?xml version="1.0" encoding="utf-8"?>
        <s:Envelope s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"
        xmlns:s="http://schemas.xmlsoap.org/soap/envelope/">
            <s:Body>%s</s:Body>
        </s:Envelope>
        """
              
# NewChannel == Argument
action = """
            <u:SetChannel xmlns:u="urn:dslforum-org:service:WLANConfiguration:1">
            <NewChannel>1</NewChannel>
            </u:SetChannel>
         """

#action2 = """
#            <u:GetDeviceLog xmlns:u="urn:dslforum-org:service:DeviceInfo:1">
#            </u:GetDeviceLog>
#          """

#            <u:GetSecurityPort xmlns:u="urn:dslforum-org:service:DeviceInfo:1">
#            </u:GetSecurityPort>
#            <u:GetInfo xmlns:u="urn:dslforum-org:service:LANConfigSecurity:1">
#            </u:GetInfo>

url = 'http://%s:%s/%s' % (FRITZ_IP_ADDRESS, FRITZ_TCP_PORT, CONTROL_URL)
print("URI: " + url)
body %= action

auth = HTTPDigestAuth(FRITZ_USERNAME, sys.argv[1])
response = requests.post(url, data=body, headers=header, auth=auth)

print(response.text)
