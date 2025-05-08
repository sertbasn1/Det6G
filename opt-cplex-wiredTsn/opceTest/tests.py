import copy

from scenario import *
from GD import *
from tools import *

def test_GD():

	G, D = generate_scenario(10, 0.15, "random", 3, 10)

	p = GD(G, O, F)
	p.prepare()
	p.solve()

	p.diagnose()

	return p.report()

