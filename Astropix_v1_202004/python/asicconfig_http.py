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
        self.url = 'http://localhost:8090/asic/'
        
        # DAC path
        self.urlSetDac = 'config/DACS/{}/set/{}'
        # Config path
        self.urlSetConfig = 'config/Config/{}/set/{}'
        
        # Injection path
        self.urlStartInjection = 'injection/start/{}/{}/{}/{}'
        self.urlStopInjection = 'injection/stop'
        
        
        # ASIC Update path
        self.urlUpdate = 'config/@send'

        # List Current DACs
        self.dacs = {'blres': 5,
                     'vn1': 2,
                     'vn2': 0,
                     'vnfb': 5,
                     'vnfoll': 1,
                     'vnfoll2': 5,
                     'vnbias': 5,
                     'vpload': 1,
                     'vncomp': 0}

        # List Config
        self.config = ['interrupt_pushpull','ResetB Biasblock']
        i = 1
        while i < 19:
            self.config.append(f'En_Inj{i}')
            i = i + 1
            
    def requestCode(self, r: requests.Response) -> bool:
        if(r.status_code != requests.codes.ok):
            print(f'HTTP Request Failed: {r.status_code}')
            return False
        else:
            return True

    def setDac(self, name: str, value: int) -> bool:
        """Set Dac with int value from 0 to 63"""
        
        if name not in self.dacs:
            print(f'{name} is not a valid dac')
            
        elif (value < 0 | value > 63): 
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
        """Get Config"""
        
        pass
    
    def resetDacs(self) -> bool:
        """Reset DACs to default value"""
        
        for key,value in self.dacs.items():
            print(key, value)
            self.setDac(key,value)
            
        return self.updateAsic()

    def updateAsic(self) -> bool:
        """Update ASIC Config"""
        
        r = requests.get(self.url + self.urlUpdate)
        
        return self.requestCode(r)

    def startInjection(self, voltage: float, delay: int, period: int, clkdiv: int) -> bool:
        """Start/Update Injection"""
        
        if ((voltage > 0) | (voltage < 1.8)):
            r = requests.get(self.url + self.urlStartInjection.format(voltage, delay, period, clkdiv))

            return self.requestCode(r)
        
        else:
            return False

    def stopInjection(self) -> bool:
        """Stop Injection"""
        
        r = requests.get(self.url + self.urlStopInjection)
        
        return self.requestCode(r)
        

        
        
