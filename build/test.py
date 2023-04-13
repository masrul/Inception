import numpy as np
from numpy import cos, sin


def get_rot_mat():
    r1, r2, r3 = np.random.random(3)
    alpha = np.pi * (2 * r1 - 1)
    beta = np.pi * (2 * r2 - 1)
    gamma = np.pi * (2 * r3 - 1)

    cos_alpha = cos(alpha)
    sin_alpha = sin(alpha)

    cos_beta = cos(beta)
    sin_beta = sin(beta)

    cos_gamma = cos(gamma)
    sin_gamma = sin(gamma)

    rot_mat = np.zeros((3, 3), dtype=float)

    rot_mat[0][0] = cos_beta * cos_gamma
    rot_mat[0][1] = sin_alpha * sin_beta * cos_gamma - cos_alpha * sin_gamma
    rot_mat[0][2] = cos_alpha * sin_beta * cos_gamma + sin_alpha * sin_gamma

    rot_mat[1][0] = cos_beta * sin_gamma
    rot_mat[1][1] = sin_alpha * sin_beta * sin_gamma + cos_alpha * cos_gamma
    rot_mat[1][2] = cos_alpha * sin_beta * sin_gamma - sin_alpha * cos_gamma

    rot_mat[2][0] = -sin_beta
    rot_mat[2][1] = sin_alpha * cos_beta
    rot_mat[2][2] = cos_alpha * cos_beta

    return rot_mat


def parse_xyz(file_name):
    with open(file_name, "r") as file:
        lines = file.readlines()
        natoms = int(lines[0])
        symbols = ["C"] * natoms
        coords = np.zeros((natoms, 3), dtype=float)
        for i, line in enumerate(lines[2:]):
            sub_strs = line.split()
            symbols[i] = sub_strs[0]
            coords[i, 0] = float(sub_strs[1])
            coords[i, 1] = float(sub_strs[2])
            coords[i, 2] = float(sub_strs[3])
    return symbols, coords


def write(file, symbols, coords):
    natoms = len(symbols)

    file.write(f"{natoms:<10d}\n\n")

    for i in range(natoms):
        file.write(
            f"{symbols[i]:4s} {coords[i][0] : 10.4f} {coords[i][1] : 10.4f} {coords[i][2] : 10.4f}\n"
        )


def main():
    file_name = "2-methyl-propanol.xyz"
    symbols, coords = parse_xyz(file_name)
    coords -= np.mean(coords, axis=0)

    file = open("traj.xyz", "w")
    for _ in range(10):
        rot_mat = get_rot_mat()
        new_coords = np.matmul(coords, rot_mat)
        write(file, symbols, new_coords)

    file.close()


main()
