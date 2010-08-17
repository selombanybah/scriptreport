#include "previewtransformer.h"

PreviewTransformer::PreviewTransformer(QTextStream *inputStream, QTextStream *outputStream) :
        in(inputStream), out(outputStream)
{
}

QTextStream* PreviewTransformer::inputStream() const {
    return in;
}

void PreviewTransformer::setInputStream(QTextStream *inputStream) {
    in = inputStream;
}

QTextStream* PreviewTransformer::outputStream() const {
    return out;
}

void PreviewTransformer::setOutputStream(QTextStream *outputStream) {
    out = outputStream;
}

bool PreviewTransformer::transform() {
    const QString startHtml = QString::fromLatin1("<html>");
    const QString endHtml = QString::fromLatin1("</html>");
    const QString separator = QString::fromLatin1("<hr>");

    if (!in || !out) {
        return false;
    }

    QString headerText;
    QTextStream header(&headerText, QIODevice::WriteOnly);
    QString contentText;
    QTextStream content(&contentText, QIODevice::WriteOnly);
    QString footerText;
    QTextStream footer(&footerText, QIODevice::WriteOnly);

    currentStream = &content;

    consume();
    if (!next.isNull()) {
        consume();
        if (next.isNull()) {
            write(current);
        } else {
            readHtml(&header, &content, &footer);
        }
    }

    header.flush();
    content.flush();
    footer.flush();

    currentStream = out;

    write(startHtml);
    if (!headerText.isEmpty()) {
        write(headerText);
        write(separator);
    } else if (!footerText.isEmpty()) {
        write(separator);
    }
    write(contentText);
    if (!footerText.isEmpty()) {
        write(separator);
        write(footerText);
    } else if (!headerText.isEmpty()) {
        write(separator);
    }
    write(endHtml);

    out->flush();
    return true;
}

void PreviewTransformer::consume() {
    current = next;
    *in >> next;
}

void PreviewTransformer::prepare() {
    consume();
    consume();
}

void PreviewTransformer::writeAndConsume() {
    *currentStream << current;
    consume();
}

void PreviewTransformer::write(const QChar &c) {
    *currentStream << c;
}

void PreviewTransformer::write(const QChar &c1, const QChar &c2) {
    *currentStream << c1 << c2;
}

void PreviewTransformer::write(const QString &s) {
    *currentStream << s;
}

void PreviewTransformer::readHtml(QTextStream *header, QTextStream *content, QTextStream *footer) {
    const QChar s1  = QChar::fromLatin1('<');
    const QChar s2  = QChar::fromLatin1('!');
    const QChar s3  = QChar::fromLatin1('-');
    const QChar s4  = QChar::fromLatin1('-');
    const QChar s5a = QChar::fromLatin1(':');
    const QChar s5b = QChar::fromLatin1('?');

    const QChar i1 = QChar::fromLatin1('?');
    const QChar i2 = QChar::fromLatin1('{');

    while (!current.isNull()) {

        if (current == i1) {
            if (next == i2) {
                readInlineConditional();
            }
        }

        if (current != s1) {
            writeAndConsume();
            continue;
        }

        if (next != s2) {
            writeAndConsume();
            continue;
        }

        consume();
        if (next != s3) {
            write(s1);
            continue;
        }

        consume();
        if (next != s4) {
            write(s1, s2);
            continue;
        }

        consume();
        if (next == s5a) {
            readChangeContext(header, content, footer);
            continue;
        } else if (next == s5b) {
            readConditional();
            continue;
        }

        readComment();
    }
}

void PreviewTransformer::readChangeContext(QTextStream *header, QTextStream *content, QTextStream *footer) {
    // header
    const QChar h1 = QChar::fromLatin1('h');
    const QChar h2 = QChar::fromLatin1('e');
    const QChar h3 = QChar::fromLatin1('a');
    const QChar h4 = QChar::fromLatin1('d');
    const QChar h5 = QChar::fromLatin1('e');
    const QChar h6 = QChar::fromLatin1('r');

    // content
    const QChar c1 = QChar::fromLatin1('c');
    const QChar c2 = QChar::fromLatin1('o');
    const QChar c3 = QChar::fromLatin1('n');
    const QChar c4 = QChar::fromLatin1('t');
    const QChar c5 = QChar::fromLatin1('e');
    const QChar c6 = QChar::fromLatin1('n');
    const QChar c7 = QChar::fromLatin1('t');

    // footer
    const QChar f1 = QChar::fromLatin1('f');
    const QChar f2 = QChar::fromLatin1('o');
    const QChar f3 = QChar::fromLatin1('o');
    const QChar f4 = QChar::fromLatin1('t');
    const QChar f5 = QChar::fromLatin1('e');
    const QChar f6 = QChar::fromLatin1('r');

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

        currentStream = header;
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

        currentStream = content;
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

        currentStream = footer;
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

void PreviewTransformer::readComment() {
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

void PreviewTransformer::readConditional() {
    const QChar a1 = QChar::fromLatin1(':');
    const QChar a2 = QChar::fromLatin1(':');

    const QChar e1 = QChar::fromLatin1('-');
    const QChar e2 = QChar::fromLatin1('-');
    const QChar e3 = QChar::fromLatin1('>');

    prepare();
    while (!current.isNull()) {

        if (current == a1) {
            if (next == a2) {
                readConditionalText();
                break;
            }
        }

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

void PreviewTransformer::readConditionalText() {
    const QChar e1 = QChar::fromLatin1('-');
    const QChar e2 = QChar::fromLatin1('-');
    const QChar e3 = QChar::fromLatin1('>');

    prepare();
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

        prepare();
        break;
    }

}

void PreviewTransformer::readInlineConditional() {
    const QChar a1 = QChar::fromLatin1(':');
    const QChar a2 = QChar::fromLatin1(':');

    const QChar e1 = QChar::fromLatin1('}');

    prepare();
    while (!current.isNull()) {

        if (current == a1) {
            if (next == a2) {
                readInlineConditionalText();
                break;
            }
        }

        if (current != e1) {
            consume();
            continue;
        }

        consume();
        break;
    }

}

void PreviewTransformer::readInlineConditionalText() {
    const QChar e1 = QChar::fromLatin1('}');

    prepare();
    while (!current.isNull()) {

        if (current != e1) {
            writeAndConsume();
            continue;
        }

        consume();
        break;
    }

}

