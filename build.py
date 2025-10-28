import os

with open("index.html", "w") as F:
    pass


def listfolders(directory, index):
    if len(os.listdir(directory)) == 0:
        with open("index.html", "a") as F:
            F.write("</div>\n")
        return

    else:
        for i in os.listdir(directory):
            print(i)
            flag = 0
            if "." not in i:
                print(f'"." is not in {i}, so it is a folder')
                flag = 1
                # print("  " * index + i)
                with open("index.html", "a") as F:
                    F.write(f"""
                        <div class="h3-folder">
                            <input
                                type="checkbox"
                                name="{i}"
                                id="{i}"
                                onchange="checkState(this)"
                            />
                            <label for="{i}">
                                <span><b>📁 {i}</b></span>
                            </label>
                                <div class="spanner">\n
                        """)
                s = directory + "/" + i
                listfolders(s, index + 1)

                with open("index.html", "a") as F:
                    F.write("</div>\n")

            if "." in i and i != ".DS_Store":
                print(f'"." is in {i}, so it is a file')
                with open("index.html", "a") as F:
                    F.write(f"""
                        <a href="{directory + "/" + i}" class="file"><h3>{i}</h3></a>
                        """)
        with open("index.html", "a") as F:
            F.write("</div>\n")


with open("index.html", "a") as F:
    F.write("""
        <!doctype html>
        <html>
            <head>
                <title>2K25-Resources</title>
                <link rel="stylesheet" href="styles.css" />
                <meta name="viewport" content="width=device-width, initial-scale=1.0" />
                <meta charset="UTF-8">
                <meta name="description" content="Resources for IIIT Hyderabad">
                <meta name="author" content="SSKTJ">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
            </head>

            <body>
                <div id="navbar">
                    <a href="index.html">
                        <h1>2K25-Resources</h1>
                    </a>
                    <div id="links">
                        <a href="semester1.html">
                            <h2 class="click-btn">Semester 1</h2>
                        </a>
                    </div>
                </div>
                <hr />\n
        """)

    F.flush()
    listfolders("Semester-1", 1)

    F.write("""
        </div>
    </div>
            """)

    F.write("""
        </body>

        <script>
            function checkState(checkbox) {
                const parentFolder = checkbox.parentElement;
                const spanner = parentFolder.querySelector(".spanner");
                if (checkbox.checked) {
                    spanner.style.display = "block";
                } else {
                    spanner.style.display = "none";
                }
            }
        </script>
    </html>
        """)
