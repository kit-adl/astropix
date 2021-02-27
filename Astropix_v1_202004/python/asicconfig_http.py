# -*- coding: utf-8 -*-
"""
Created on Sat Jan 30 13:31:13 2021

@author: Nicolas Striebig

Python Interface to qt-httpserver
"""

import requests


class asicconfig_http:

    def __init__(self):
        
        # Base Url
        self.url = 'http://localhost:8090/asic/config/'
        
        # DAC path
        self.urlSetDac = 'DACS/{}/set/{}'
        # Config path
        self.urlSetConfig = 'Config/{}/set/{}'
        # ASIC Update path
        self.urlUpdate = '@send'

        # List Current DACs
        self.dacs = ['blres',
                     'vn1',
                     'vn2',
                     'vnfb',
                     'vnfoll',
                     'vnfoll2',
                     'vnbias',
                     'vpload',
                     'vncomp']

        # List Config
        self.config = ['interrupt_pushpull','ResetB Biasblock']
        i = 1
        while i < 19:
            self.config.append(f'En_Inj{i}')
            i = i + 1
            
    def requestCode(r: requests.Response) -> bool:
        if(r.status_code != requests.codes.ok):
            print(f'HTTP Request Failed: {r.status_code}')
            return False
        else:
            return True

    def setDac(self, name: str, value: int) -> bool:
        """Set Dac with int value from 0 to 63"""
        
        if name not in self.dacs:
            print(f'{name} is not a valid dac')
            
        elif (value >= 0 | value <= 63): 
            print(f'{value} is not a valid dac value')
        
        else:
            r = requests.get(self.url + self.urlSetDac.format(name, value))

            return self.requestCode(r)
        
        return False

    def setConfig(self, name: str, value: bool) -> bool:
        """Set Config with bool value"""
        
        if name not in self.config:
            print(f'{name} is not a valid config')
        
        else:
            r = requests.get(self.url + self.urlSetConfig.format(name, value))

            return self.requestCode(r)

        return False

    def getDac(self, name: str):
        # TODO
        print("not implemented")

    def updateAsic(self) -> bool:
        """Update ASIC Config"""
        
        r = requests.get(self.url + self.urlUpdate)
        
        return self.requestCode(r)
