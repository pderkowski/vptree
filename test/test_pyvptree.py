from __future__ import print_function

import os
import sys
if len(sys.argv) > 1 and sys.argv[1]:
    sys.path.insert(0, os.path.abspath(sys.argv[1]))
else:
    print("Provide a path to the directory containing vptree module.")
    sys.exit(2)

import vptree
import unittest
import math
import random

try:
    import numpy as np
    runNumpyTests = True
except ImportError:
    print(ImportError('Could not import numpy, skipping some tests.'))
    runNumpyTests = False

def getPoints(count, size):
    return [[random.random() for _ in range(size)] for _ in range(count)]

class TestVpTree(unittest.TestCase):
    def test_constructorFromListOfLists(self):
        try:
            points = [[0, 1], [2., 3.]]
            tree = vptree.VpTree(points)
        except:
            self.fail("VpTree was not constructed properly")

    def test_simpleQuery(self):
        points = [
            [0., 1., 2.],
            [2., 2., 2.],
            [3.5, 0., -1],
            [0, 0, 0.5],
            [-0.5, -0.5, 0.5]
        ]

        tree = vptree.VpTree(points)

        distances, indices = tree.getNearestNeighbors([0., 0., 0.5], 3)

        self.assertEqual(len(distances), 3)
        self.assertEqual(len(indices), 3)

        self.assertEqual(distances, [0, math.sqrt(0.5), math.sqrt(3.25)])
        self.assertEqual(indices, [3, 4, 0])

    def test_parallelQuery(self):
        points = getPoints(1000, 100)

        tree = vptree.VpTree(points)
        batch = tree.getNearestNeighborsBatch(points, 10)

        self.assertEqual(batch[0], tree.getNearestNeighbors(points[0], 10))

class TestVpTreeWithNumpy(unittest.TestCase):
    def test_constructorFromArrayOfFloat(self):
        points = np.array([
            [0., 1., 2.],
            [2., 2., 2.],
            [3.5, 0., -1],
            [0, 0, 0.5],
            [-0.5, -0.5, 0.5]
        ], np.dtype('f'))

        tree = vptree.VpTree(points)

        distances, indices = tree.getNearestNeighbors([0., 0., 0.5], 3)

        self.assertEqual(len(distances), 3)
        self.assertEqual(len(indices), 3)

        self.assertEqual(distances, [0, math.sqrt(0.5), math.sqrt(3.25)])
        self.assertEqual(indices, [3, 4, 0])

    def test_constructorFromArrayOfDouble(self):
        points = np.array([
            [0., 1., 2.],
            [2., 2., 2.],
            [3.5, 0., -1],
            [0, 0, 0.5],
            [-0.5, -0.5, 0.5]
        ], np.dtype('d'))

        tree = vptree.VpTree(points)

        distances, indices = tree.getNearestNeighbors([0., 0., 0.5], 3)

        self.assertEqual(len(distances), 3)
        self.assertEqual(len(indices), 3)

        self.assertEqual(distances, [0, math.sqrt(0.5), math.sqrt(3.25)])
        self.assertEqual(indices, [3, 4, 0])


if __name__ == '__main__':
    suite = unittest.TestSuite()
    suite.addTest(unittest.makeSuite(TestVpTree))
    if runNumpyTests:
        suite.addTest(unittest.makeSuite(TestVpTreeWithNumpy))

    unittest.TextTestRunner().run(suite)
