import unittest
import creature
import numpy as np


class TestCreature(unittest.TestCase):
    def test_creatureExist(self):
        self.assertIsNotNone(creature.Creature)

    def test_creatureGetFlatLinks(self):
        c = creature.Creature(gene_count = 4)
        links = c.get_flat_links()
        self.assertEqual(len(links),4)


    def test_ExpLinks(self):
        for i in range(100):
            c = creature.Creature(gene_count = 25)
            links = c.get_flat_links()
            exp_links = c.get_expanded_links()
            print(len(exp_links))
            self.assertGreaterEqual(len(exp_links),len(links))


unittest.main()
