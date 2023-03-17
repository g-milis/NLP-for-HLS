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


if __name__ == '__main__':
    """From https://matplotlib.org/stable/gallery/user_interfaces/svg_tooltip_sgskip.html"""
    import matplotlib.pyplot as plt
    import xml.etree.ElementTree as ET
    from io import BytesIO

    ET.register_namespace("", "http://www.w3.org/2000/svg")

    fig, ax = plt.subplots()

    # Create patches to which tooltips will be assigned.
    rect1 = plt.Rectangle((10, -20), 10, 5, fc='blue')
    rect2 = plt.Rectangle((-20, 15), 10, 5, fc='green')

    shapes = [rect1, rect2]
    labels = ['This is a blue rectangle.', 'This is a green rectangle']

    for i, (item, label) in enumerate(zip(shapes, labels)):
        patch = ax.add_patch(item)
        annotate = ax.annotate(labels[i], xy=item.get_xy(), xytext=(0, 0),
                            textcoords='offset points', color='w', ha='center',
                            fontsize=8, bbox=dict(boxstyle='round, pad=.5',
                                                    fc=(.1, .1, .1, .92),
                                                    ec=(1., 1., 1.), lw=1,
                                                    zorder=1))

        ax.add_patch(patch)
        patch.set_gid(f'mypatch_{i:03d}')
        annotate.set_gid(f'mytooltip_{i:03d}')

    # Save the figure in a fake file object
    ax.set_xlim(-30, 30)
    ax.set_ylim(-30, 30)
    ax.set_aspect('equal')

    f = BytesIO()
    plt.savefig(f, format="svg")

    # --- Add interactivity ---

    # Create XML tree from the SVG file.
    tree, xmlid = ET.XMLID(f.getvalue())
    tree.set('onload', 'init(event)')

    for i in shapes:
        # Get the index of the shape
        index = shapes.index(i)
        # Hide the tooltips
        tooltip = xmlid[f'mytooltip_{index:03d}']
        tooltip.set('visibility', 'hidden')
        # Assign onmouseover and onmouseout callbacks to patches.
        mypatch = xmlid[f'mypatch_{index:03d}']
        mypatch.set('onmouseover', "ShowTooltip(this)")
        mypatch.set('onmouseout', "HideTooltip(this)")

    # This is the script defining the ShowTooltip and HideTooltip functions.
    script = """
        <script type="text/ecmascript">
        <![CDATA[

        function init(event) {
            if ( window.svgDocument == null ) {
                svgDocument = event.target.ownerDocument;
                }
            }

        function ShowTooltip(obj) {
            var cur = obj.id.split("_")[1];
            var tip = svgDocument.getElementById('mytooltip_' + cur);
            tip.setAttribute('visibility', "visible")
            }

        function HideTooltip(obj) {
            var cur = obj.id.split("_")[1];
            var tip = svgDocument.getElementById('mytooltip_' + cur);
            tip.setAttribute('visibility', "hidden")
            }

        ]]>
        </script>
        """

    # Insert the script at the top of the file and save it.
    tree.insert(0, ET.XML(script))
    ET.ElementTree(tree).write('svg_tooltip.svg')
