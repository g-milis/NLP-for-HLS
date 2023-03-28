""" Contains plotting and visualization utility functions. """
import mplcursors
import numpy as np
from io import BytesIO
import matplotlib.pyplot as plt
import xml.etree.ElementTree as ET


def k_means_plot(data, labels, centroids, text, title, filepath, show=False):
    plt.figure()
    u_labels = np.unique(labels)
    for i in u_labels:
        scatter = plt.scatter(data[labels == i, 0], data[labels == i, 1])
        cursor = mplcursors.cursor(scatter, hover=True)
    
        # For annotation on hover
        @cursor.connect("add")
        def on_add(sel):
            sel.annotation.set(text=text[sel.index])

    # Plotting the Centroids
    plt.scatter(centroids[:, 0], centroids[:, 1], s=100, color='k', marker='P')
    plt.grid()
    plt.title(title)
    plt.xlabel("Component 1")
    plt.ylabel("Component 2")
    plt.savefig(filepath, bbox_inches='tight')
    if show: plt.show()


def interactive_svg(data, text, title, filename):
    """ From https://matplotlib.org/stable/gallery/user_interfaces/svg_tooltip_sgskip.html """

    ET.register_namespace("", "http://www.w3.org/2000/svg")
    x, y, z = data[:, 0], data[:, 1], data[:, 2]
    plt.figure()
    # Scatter data in color
    color_scatter = plt.scatter(x, y, c=z)   
    bar = plt.colorbar(color_scatter)
    plt.grid()
    plt.title(title)
    plt.xlabel("Component 1")
    plt.ylabel("Component 2")
    bar.ax.set_ylabel("Component 3")
    # Save a simple pdf plot without clutter
    plt.savefig(filename + '.pdf', bbox_inches='tight')
    
    # Scatter data in almost-transparent points to add annotations
    for i, label in enumerate(text):
        scatter = plt.scatter(x[i], y[i], c='b', alpha=0.01)
        annotate = plt.annotate(label, xy=np.array([x[i] + 0.05, y[i] + 0.05]))
        scatter.set_gid(f'point_{i:03d}')
        annotate.set_gid(f'tooltip_{i:03d}')
    # Save the figure in a fake file object
    f = BytesIO()
    plt.savefig(f, format="svg")

    # Create XML tree from the SVG file.
    tree, xmlid = ET.XMLID(f.getvalue())
    tree.set('onload', 'init(event)')
    for i in range(len(text)):
        # Hide the tooltips
        tooltip = xmlid[f'tooltip_{i:03d}']
        tooltip.set('visibility', 'hidden')
        # Assign onmouseover and onmouseout callbacks to patches.
        mypatch = xmlid[f'point_{i:03d}']
        mypatch.set('onmouseover', "ShowTooltip(this)")
        mypatch.set('onmouseout', "HideTooltip(this)")

    # This is the script defining the ShowTooltip and HideTooltip functions.
    script = """
        <script type="text/ecmascript">
        <![CDATA[

        function init(event) {
            if (window.svgDocument == null) {
                svgDocument = event.target.ownerDocument;
            }
        }

        function ShowTooltip(obj) {
            var cur = obj.id.split("_")[1];
            var tip = svgDocument.getElementById('tooltip_' + cur);
            tip.setAttribute('visibility', "visible")
        }

        function HideTooltip(obj) {
            var cur = obj.id.split("_")[1];
            var tip = svgDocument.getElementById('tooltip_' + cur);
            tip.setAttribute('visibility', "hidden")
        }

        ]]>
        </script>
        """
    # Insert the script at the top of the file and save it.
    tree.insert(0, ET.XML(script))
    ET.ElementTree(tree).write(filename + '.svg')
