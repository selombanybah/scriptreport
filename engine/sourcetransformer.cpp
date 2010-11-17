#include "sourcetransformer.h"

class SourceTransformerPrivate {
public:
    SourceTransformerPrivate(QTextStream *inputStream, QTextStream *outputStream) :
            in(inputStream), out(outputStream) {}

    bool transform();

    void consume();
    void prepare();
    void writeAndConsume();
    void writeHtmlAndConsume();
    void writeStartHtml();
    void writeEndHtml();
    void write(const QChar &c);
    void write(const QChar &c1, const QChar &c2);
    void write(const QString &s);

    void writeHtmlChar(const QChar &c);

    void readHtml();
    void readScript(bool writeEnd);
    void readInLineScript();
    void readChangeContext();
    void readHtmlComment();
    void readConditional();
    void readConditionalText();
    void readInlineConditional();
    void readInlineConditionalText();
    void ajust();

    QChar current;
    QChar next;
    bool startHtmlWrited;
    QTextStream *in;
    QTextStream *out;
    int inLine;
    int inColumn;
    int outLine;
    int outColumn;

};

SourceTransformer::SourceTransformer(QTextStream *inputStream, QTextStream *outputStream) :
        d(new SourceTransformerPrivate(inputStream, outputStream))
{
}

SourceTransformer::~SourceTransformer() {
    delete d;
}

QTextStream* SourceTransformer::inputStream() const {
    return d->in;
}

void SourceTransformer::setInputStream(QTextStream *inputStream) {
    d->in = inputStream;
}

QTextStream* SourceTransformer::outputStream() const {
    return d->out;
}

void SourceTransformer::setOutputStream(QTextStream *outputStream) {
    d->out = outputStream;
}

bool SourceTransformer::transform() {
    return d->transform();
}

/*
 * Private members
 */

bool SourceTransformerPrivate::transform() {
    const QString first = QString::fromLatin1("var _ = sr.report.writeContent;\nvar _f = sr.report.isFinal;");
    inLine    = 0;
    inColumn  = 0;
    outLine   = 0;
    outColumn = 0;
    current = QChar::fromLatin1(' ');
    next = QChar::fromLatin1(' ');
    startHtmlWrited = false;

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

void SourceTransformerPrivate::consume() {
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

void SourceTransformerPrivate::prepare() {
    consume();
    consume();
}

void SourceTransformerPrivate::writeAndConsume() {
    write(current);
    consume();
}

void SourceTransformerPrivate::writeHtmlAndConsume() {
    writeStartHtml();
    writeHtmlChar(current);
    consume();
}

void SourceTransformerPrivate::writeStartHtml() {
    const QString start = QString::fromLatin1("_(\"");
    if (!startHtmlWrited) {
        write(start);
        startHtmlWrited = true;
    }
}

void SourceTransformerPrivate::writeEndHtml() {
    const QString end = QString::fromLatin1("\");");

    if (startHtmlWrited) {
        write(end);
        startHtmlWrited = false;
    }
}

void SourceTransformerPrivate::write(const QChar &c) {
    const QChar n = QChar::fromLatin1('\n');

    *out << c;
    if (c == n) {
        outLine++;
        outColumn = 0;
    } else {
        outColumn++;
    }
}

void SourceTransformerPrivate::write(const QChar &c1, const QChar &c2) {
    write(c1);
    write(c2);
}

void SourceTransformerPrivate::write(const QString &s) {
    for (int i = 0; i < s.size(); i++) {
        write(s[i]);
    }
}

void SourceTransformerPrivate::writeHtmlChar(const QChar &c) {
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

void SourceTransformerPrivate::readHtml() {
    const QChar s1  = QChar::fromLatin1('<');
    const QChar s2  = QChar::fromLatin1('!');
    const QChar s3  = QChar::fromLatin1('-');
    const QChar s4  = QChar::fromLatin1('-');
    const QChar s5  = QChar::fromLatin1('@');
    const QChar s5b = QChar::fromLatin1('$');
    const QChar s5c = QChar::fromLatin1(':');
    const QChar s5d = QChar::fromLatin1('?');

    const QChar as1  = QChar::fromLatin1('$');
    const QChar as1b = QChar::fromLatin1('?');
    const QChar as2  = QChar::fromLatin1('{');

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
        } else if (current == as1b) {
            if (next == as2) {
                writeEndHtml();
                write(astart);
                readInlineConditional();
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
        } else if (next == s5d) {
            // <!--?
            writeEndHtml();
            write(astart);
            readConditional();
            write(aend);
            continue;
        } else {
            // <!--Comment
            readHtmlComment();
        }
    }

    writeEndHtml();
}

void SourceTransformerPrivate::readScript(bool writeEnd) {
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

void SourceTransformerPrivate::readInLineScript() {
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

void SourceTransformerPrivate::readChangeContext() {
    // header
    const QChar h1 = QChar::fromLatin1('h');
    const QChar h2 = QChar::fromLatin1('e');
    const QChar h3 = QChar::fromLatin1('a');
    const QChar h4 = QChar::fromLatin1('d');
    const QChar h5 = QChar::fromLatin1('e');
    const QChar h6 = QChar::fromLatin1('r');
    const QString header = QString::fromLatin1("_ = sr.report.writeHeader;");
    const QString headerFirst = QString::fromLatin1("_ = sr.report.writeHeaderFirst;");
    const QString headerLast = QString::fromLatin1("_ = sr.report.writeHeaderLast;");

    // content
    const QChar c1 = QChar::fromLatin1('c');
    const QChar c2 = QChar::fromLatin1('o');
    const QChar c3 = QChar::fromLatin1('n');
    const QChar c4 = QChar::fromLatin1('t');
    const QChar c5 = QChar::fromLatin1('e');
    const QChar c6 = QChar::fromLatin1('n');
    const QChar c7 = QChar::fromLatin1('t');
    const QString content = QString::fromLatin1("_ = sr.report.writeContent;");

    // footer
    const QChar f1 = QChar::fromLatin1('f');
    const QChar f2 = QChar::fromLatin1('o');
    const QChar f3 = QChar::fromLatin1('o');
    const QChar f4 = QChar::fromLatin1('t');
    const QChar f5 = QChar::fromLatin1('e');
    const QChar f6 = QChar::fromLatin1('r');
    const QString footer = QString::fromLatin1("_ = sr.report.writeFooter;");
    const QString footerFirst = QString::fromLatin1("_ = sr.report.writeFooterFirst;");
    const QString footerLast = QString::fromLatin1("_ = sr.report.writeFooterLast;");

    // First
    const QChar b1 = QChar::fromLatin1('F');
    const QChar b2 = QChar::fromLatin1('i');
    const QChar b3 = QChar::fromLatin1('r');
    const QChar b4 = QChar::fromLatin1('s');
    const QChar b5 = QChar::fromLatin1('t');

    // Last
    const QChar l1 = QChar::fromLatin1('L');
    const QChar l2 = QChar::fromLatin1('a');
    const QChar l3 = QChar::fromLatin1('s');
    const QChar l4 = QChar::fromLatin1('t');

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

        consume();
        if (next == b1) {
            // First

            consume();
            if (next != b2) {
                goto end;
            }

            consume();
            if (next != b3) {
                goto end;
            }

            consume();
            if (next != b4) {
                goto end;
            }

            consume();
            if (next != b5) {
                goto end;
            }

            write(headerFirst);
        } else if (next == l1) {
            // Last

            consume();
            if (next != l2) {
                goto end;
            }

            consume();
            if (next != l3) {
                goto end;
            }

            consume();
            if (next != l4) {
                goto end;
            }

            write(headerLast);
        } else {
            write(header);
        }

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

        consume();
        if (next == b1) {
            // First

            consume();
            if (next != b2) {
                goto end;
            }

            consume();
            if (next != b3) {
                goto end;
            }

            consume();
            if (next != b4) {
                goto end;
            }

            consume();
            if (next != b5) {
                goto end;
            }

            write(footerFirst);
        } else if (next == l1) {
            // Last

            consume();
            if (next != l2) {
                goto end;
            }

            consume();
            if (next != l3) {
                goto end;
            }

            consume();
            if (next != l4) {
                goto end;
            }

            write(footerLast);
        } else {
            write(footer);
        }
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

void SourceTransformerPrivate::readHtmlComment() {
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

void SourceTransformerPrivate::readConditional() {
    const QString s = QString::fromLatin1("_f?");
    const QString m = QString::fromLatin1(":");
    const QString e = QString::fromLatin1("\"\"");

    const QChar a1 = QChar::fromLatin1(':');
    const QChar a2 = QChar::fromLatin1(':');

    const QChar e1 = QChar::fromLatin1('-');
    const QChar e2 = QChar::fromLatin1('-');
    const QChar e3 = QChar::fromLatin1('>');

    prepare();
    write(s);
    ajust();
    int i = -1;
    while (!current.isNull()) {
        ++i;

        if (current == a1) {
            if (next == a2) {
                if (i <= 0) {
                    write(e);
                }
                write(m);
                readConditionalText();
                break;
            }
        }

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
            writeAndConsume();
            continue;
        }

        prepare();
        write(m);
        write(e);
        break;
    }

}

void SourceTransformerPrivate::readConditionalText() {
    const QString s = QString::fromLatin1("\"");
    const QString e = QString::fromLatin1("\"");

    const QChar e1 = QChar::fromLatin1('-');
    const QChar e2 = QChar::fromLatin1('-');
    const QChar e3 = QChar::fromLatin1('>');

    prepare();
    write(s);
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
            write(e1);
            continue;
        }

        prepare();
        write(e);
        break;
    }

}

void SourceTransformerPrivate::readInlineConditional() {
    const QString s = QString::fromLatin1("_f?");
    const QString m = QString::fromLatin1(":");
    const QString e = QString::fromLatin1("\"\"");


    const QChar a1 = QChar::fromLatin1(':');
    const QChar a2 = QChar::fromLatin1(':');

    const QChar e1 = QChar::fromLatin1('}');

    prepare();
    write(s);
    ajust();
    int i = -1;
    while (!current.isNull()) {
        ++i;

        if (current == a1) {
            if (next == a2) {                
                if (i <= 0) {
                    write(e);
                }
                write(m);
                readInlineConditionalText();
                break;
            }
        }

        if (current != e1) {
            writeAndConsume();
            continue;
        }

        consume();
        write(m);
        write(e);
        break;
    }

}

void SourceTransformerPrivate::readInlineConditionalText() {
    const QString s = QString::fromLatin1("\"");
    const QString e = QString::fromLatin1("\"");

    const QChar e1 = QChar::fromLatin1('}');

    prepare();
    write(s);
    while (!current.isNull()) {

        if (current != e1) {
            writeAndConsume();
            continue;
        }

        consume();
        write(e);
        break;
    }

}

void SourceTransformerPrivate::ajust() {
    const QChar n = QChar::fromLatin1('\n');
    const QChar sp = QChar::fromLatin1(' ');

    if (current == n) {
        while (outLine < inLine - 1) {
            write(n);
        }
    } else {
        while (outLine < inLine) {
            write(n);
        }
    }

    while (outColumn < inColumn - 1) {
        write(sp);
    }
}
