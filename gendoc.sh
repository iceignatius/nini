#!/bin/sh

#
# Generate documents.
#

doxygen

sed -i -e 's,begin{document},usepackage{CJKutf8}\n\\begin{document}\n\\begin{CJK}{UTF8}{bsmi},' \
    doc/latex/refman.tex
sed -i -e 's,end{document},end{CJK}\n\\end{document},' \
    doc/latex/refman.tex

cd doc/latex && \
    make && \
    cp refman.pdf "../Nested INI Library Manual.pdf"
