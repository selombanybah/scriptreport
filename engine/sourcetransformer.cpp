#include "sourcetransformer.h"

SourceTransformer::SourceTransformer(QTextStream *inputStream, QTextStream *outputStream) :
        in(inputStream), out(outputStream)
{
}

QTextStream* SourceTransformer::inputStream() const {
    return in;
}

void SourceTransformer::setInputStream(QTextStream *inputStream) {
    in = inputStream;
}

QTextStream* SourceTransformer::outputStream() const {
    return out;
}

void SourceTransformer::setOutputStream(QTextStream *outputStream) {
    out = outputStream;
}

bool SourceTransformer::transform() {
    const QString first = QString::fromLatin1("var _ = report.writeContent;");
    inLine    = 0;
    inColumn  = 0;
    outLine   = 0;
    outColumn = 0;

    if (!in || !out) {
        return false;
    }

    write(first);

    *in >> next;
    if (next.isNull()) {
        return true;
    }
    consume();
    if (next.isNull()) {
        writeStartHtml();
        writeHtmlChar(current);
        writeEndHtml();
        out->flush();
        return true;
    }
    readHtml();
    out->flush();

    return true;
}

void SourceTransformer::consume() {
    const QChar n = QChar::fromLatin1('\n');

    current = next;
    *in >> next;
    if (current == n) {
        inLine++;
        inColumn = 0;
    } else {
        inColumn++;
    }
}

void SourceTransformer::prepare() {
    consume();
    consume();
}

void SourceTransformer::writeAndConsume() {
    write(current);
    consume();
}

void SourceTransformer::writeHtmlAndConsume() {
    writeStartHtml();
    writeHtmlChar(current);
    consume();
}

void SourceTransformer::writeStartHtml() {
    const QString start = QString::fromLatin1("_(\"");
    if (!startHtmlWrited) {
        write(start);
        startHtmlWrited = true;
    }
}

void SourceTransformer::writeEndHtml() {
    const QString end = QString::fromLatin1("\");");

    if (startHtmlWrited) {
        write(end);
        startHtmlWrited = false;
    }
}

void SourceTransformer::write(const QChar &c) {
    const QChar n = QChar::fromLatin1('\n');

    *out << c;
    if (c == n) {
        outLine++;
        outColumn = 0;
    } else {
        outColumn++;
    }
}

void SourceTransformer::write(const QChar &c1, const QChar &c2) {
    write(c1);
    write(c2);
}

void SourceTransformer::write(const QString &s) {
    for (int i = 0; i < s.size(); i++) {
        write(s[i]);
    }
}

void SourceTransformer::writeHtmlChar(const QChar &c) {
    const QChar r = QChar::fromLatin1('\r');
    const QChar n = QChar::fromLatin1('\n');
    const QChar q = QChar::fromLatin1('"');
    const QChar s = QChar::fromLatin1('\\');

    const QString r_ = QString::fromLatin1("\\r");
    const QString n_ = QString::fromLatin1("\\n");
    const QString q_ = QString::fromLatin1("\\\"");
    const QString s_ = QString::fromLatin1("\\\\");

    if (c == r) {
        write(r_);
    } else if (c == n) {
        write(n_);
    } else if (c == q) {
        write(q_);
    } else if (c == s) {
        write(s_);
    } else {
        write(c);
    }
}

void SourceTransformer::readHtml() {
    const QChar s1  = QChar::fromLatin1('<');
    const QChar s2  = QChar::fromLatin1('!');
    const QChar s3  = QChar::fromLatin1('-');
    const QChar s4  = QChar::fromLatin1('-');
    const QChar s5  = QChar::fromLatin1('@');
    const QChar s5b = QChar::fromLatin1('$');
    const QChar s5c = QChar::fromLatin1(':');

    const QChar as1 = QChar::fromLatin1('$');
    const QChar as2 = QChar::fromLatin1('{');

    const QString astart = QString::fromLatin1("_(");
    const QString aend   = QString::fromLatin1(");");

    startHtmlWrited = false;

    while (!current.isNull()) {
        if (current == as1) {
            if (next == as2) {
                writeEndHtml();
                write(astart);
                readInLineScript();
                write(aend);
                continue;
            } else {
                writeHtmlAndConsume();
                continue;
            }
        } else if (current != s1) {
            writeHtmlAndConsume();
            continue;
        }

        if (next != s2) {
            writeHtmlAndConsume();
            continue;
        }

        consume();
        if (next != s3) {
            writeStartHtml();
            write(s1);
            continue;
        }

        consume();
        if (next != s4) {
            writeStartHtml();
            write(s1, s2);
            continue;
        }

        consume();
        if (next == s5) {
            // <!--@
            writeEndHtml();
            readScript(true);
            continue;
        } else if (next == s5b) {
            // <!--$
            writeEndHtml();
            write(astart);
            readScript(false);
            write(aend);
            continue;
        } else if (next == s5c) {
            // <!--:
            writeEndHtml();
            readChangeContext();
        } else {
            // <!--Comment
            readHtmlComment();
        }
    }

    writeEndHtml();
}

void SourceTransformer::readScript(bool writeEnd) {
    const QChar e1 = QChar::fromLatin1('-');
    const QChar e2 = QChar::fromLatin1('-');
    const QChar e3 = QChar::fromLatin1('>');

    const QChar end = QChar::fromLatin1(';');

    prepare();
    ajust();
    while (!current.isNull()) {
        if (current != e1) {
            writeAndConsume();
            continue;
        }

        if (next != e2) {
            writeAndConsume();
            continue;
        }

        consume();
        if (next != e3) {
            write(e1);
            continue;
        }

        if (writeEnd) {
            write(end);
        }

        prepare();
        break;
    }
}

void SourceTransformer::readInLineScript() {
    const QChar ae1 = QChar::fromLatin1('}');

    prepare();
    ajust();
    while (!current.isNull()) {
        if (current != ae1) {
            writeAndConsume();
            continue;
        }

        consume();
        break;
    }
}

void SourceTransformer::readChangeContext() {
    // header
    const QChar h1 = QChar::fromLatin1('h');
    const QChar h2 = QChar::fromLatin1('e');
    const QChar h3 = QChar::fromLatin1('a');
    const QChar h4 = QChar::fromLatin1('d');
    const QChar h5 = QChar::fromLatin1('e');
    const QChar h6 = QChar::fromLatin1('r');
    const QString header = QString::fromLatin1("_ = report.writeHeader;");


    // content
    const QChar c1 = QChar::fromLatin1('c');
    const QChar c2 = QChar::fromLatin1('o');
    const QChar c3 = QChar::fromLatin1('n');
    const QChar c4 = QChar::fromLatin1('t');
    const QChar c5 = QChar::fromLatin1('e');
    const QChar c6 = QChar::fromLatin1('n');
    const QChar c7 = QChar::fromLatin1('t');
    const QString content = QString::fromLatin1("_ = report.writeContent;");

    // footer
    const QChar f1 = QChar::fromLatin1('f');
    const QChar f2 = QChar::fromLatin1('o');
    const QChar f3 = QChar::fromLatin1('o');
    const QChar f4 = QChar::fromLatin1('t');
    const QChar f5 = QChar::fromLatin1('e');
    const QChar f6 = QChar::fromLatin1('r');
    const QString footer = QString::fromLatin1("_ = report.writeFooter;");

    prepare();
    if (current == h1) {
        if (next != h2) {
            goto end;
        }

        consume();
        if (next != h3) {
            goto end;
        }

        consume();
        if (next != h4) {
            goto end;
        }

        consume();
        if (next != h5) {
            goto end;
        }

        consume();
        if (next != h6) {
            goto end;
        }

        write(header);
    } else if (current == c1) {
        if (next != c2) {
            goto end;
        }

        consume();
        if (next != c3) {
            goto end;
        }

        consume();
        if (next != c4) {
            goto end;
        }

        consume();
        if (next != c5) {
            goto end;
        }

        consume();
        if (next != c6) {
            goto end;
        }

        consume();
        if (next != c7) {
            goto end;
        }

        write(content);
    } else if (current == f1) {
        if (next != f2) {
            goto end;
        }

        consume();
        if (next != f3) {
            goto end;
        }

        consume();
        if (next != f4) {
            goto end;
        }

        consume();
        if (next != f5) {
            goto end;
        }

        consume();
        if (next != f6) {
            goto end;
        }

        write(footer);
    }

 end:
    const QChar e1 = QChar::fromLatin1('-');
    const QChar e2 = QChar::fromLatin1('-');
    const QChar e3 = QChar::fromLatin1('>');

    while (!current.isNull()) {
        if (current != e1) {
            consume();
            continue;
        }

        if (next != e2) {
            consume();
            continue;
        }

        consume();
        if (next != e3) {
            consume();
            continue;
        }

        prepare();
        break;
    }
}

void SourceTransformer::readHtmlComment() {
    const QChar e1 = QChar::fromLatin1('-');
    const QChar e2 = QChar::fromLatin1('-');
    const QChar e3 = QChar::fromLatin1('>');

    prepare();
    while (!current.isNull()) {
        if (current != e1) {
            consume();
            continue;
        }

        if (next != e2) {
            consume();
            continue;
        }

        consume();
        if (next != e3) {
            consume();
            continue;
        }

        prepare();
        break;
    }
}

void SourceTransformer::ajust() {
    const QChar n = QChar::fromLatin1('\n');
    const QChar sp = QChar::fromLatin1(' ');

    while (outLine < inLine) {
        write(n);
    }

    while (outColumn < inColumn - 1) {
        write(sp);
    }
}
