

__all__ = ['DEFAULT_MODEL', 'AMELET_MODEL', 'Amelet_1_0_0']

from amelet_1_0_0.amelet import Amelet
Amelet_1_0_0 = Amelet


from node import load

AMELET_1_0_0_MODEL = load(Amelet)
AMELET_MODEL = AMELET_1_0_0_MODEL
DEFAULT_MODEL = AMELET_MODEL


import api
__all__ += api.__all__
from api import *

import node
__all__ += node.__all__
from node import *
