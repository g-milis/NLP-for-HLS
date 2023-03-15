import mplcursors
import matplotlib.pyplot as plt


def annotated_plot(data, text, title, filepath):
    x, y, z = data[:, 0], data[:, 1], data[:, 2]
    plt.figure()
    scatter = plt.scatter(x, y, c=z)
    plt.grid()
    bar = plt.colorbar(scatter)
    plt.xlabel("Component 1")
    plt.ylabel("Component 2")
    bar.ax.set_ylabel("Component 3")
    cursor = mplcursors.cursor(scatter, hover=True)
    
    # For annotation on hover
    @cursor.connect("add")
    def on_add(sel):
        sel.annotation.set(text=text[sel.index])

    plt.title(title)
    plt.savefig(filepath, bbox_inches='tight')
