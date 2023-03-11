import matplotlib.pyplot as plt
from matplotlib import rc

rc('font',**{'family':'serif','serif':['Palatino']})
plt.rcParams['pdf.fonttype'] = 42

SAVE_IMAGE = True


def visualize(input_file):
    points_description = dict()
    pairs = []
    x_blue, y_blue, x_red, y_red = [], [], [], []
    file = open(input_file, 'r')
    file.readline()
    for line in file.readlines():
        if line.strip():
            point_id, x, y, color = line.strip().split()
            points_description[point_id] = (float(x), float(y))
            if color == '0':
                x_blue.append(float(x))
                y_blue.append(float(y))
            else:
                x_red.append(float(x))
                y_red.append(float(y))
    file.close()
    file = open('result.txt', 'r')
    file.readline()
    file.readline()
    for line in file.readlines():
        if line.strip():
            pairs.append(tuple(line.strip().split()))
    file.close()

    fig = plt.figure(figsize=(18, 8))
    plt.scatter(x_blue, y_blue, color='blue', label='Медведи')
    plt.scatter(x_red, y_red, color='red', label='Медведицы')
    for point_id1, point_id2 in pairs:
        x = [points_description[point_id1][0], points_description[point_id2][0]]
        y = [points_description[point_id1][1], points_description[point_id2][1]]
        plt.plot(x, y, color='black', ms=0)
    plt.title('Маршруты между медведями и медведицами', fontsize=20)
    plt.xlabel('x', fontsize=15)
    plt.ylabel('y', fontsize=15)
    plt.legend(prop={'size': 14})
    plt.grid()
    if SAVE_IMAGE:
        fig.savefig('result.pdf', format='pdf', dpi=600)
    plt.show()


if __name__ == '__main__':
    input_file_name = input('Input path to data file...\n')
    visualize(input_file_name)
