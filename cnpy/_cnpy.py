# -*- coding: utf-8 -*-

import cnpy.cnpy_c as cnpy_c
import numpy as np


class CNPY:
    def __init__(self):
        pass

    def calc(self):
        arr_in = np.linspace(0, 100)
        arr_out = cnpy_c.process_arr(arr_in)
        return arr_out
