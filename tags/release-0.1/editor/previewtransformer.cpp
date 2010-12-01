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
    QString headerFirstText;
    QTextStream headerFirst(&headerFirstText, QIODevice::WriteOnly);
    QString headerLastText;
    QTextStream headerLast(&headerLastText, QIODevice::WriteOnly);
    QString contentText;
    QTextStream content(&contentText, QIODevice::WriteOnly);
    QString footerText;
    QTextStream footer(&footerText, QIODevice::WriteOnly);
    QString footerFirstText;
    QTextStream footerFirst(&footerFirstText, QIODevice::WriteOnly);
    QString footerLastText;
    QTextStream footerLast(&footerLastText, QIODevice::WriteOnly);

    currentStream = &content;

    consume();
    if (!next.isNull()) {
        consume();
        if (next.isNull()) {
            write(current);
        } else {
            readHtml(&headerFirst, &header, &headerLast, &content, &footerFirst, &footer, &footerLast);
        }
    }

    header.flush();
    headerFirst.flush();
    headerLast.flush();
    content.flush();
    footer.flush();
    footerFirst.flush();
    footerLast.flush();

    currentStream = out;

    bool hasHeader = !headerText.isEmpty() || !headerFirstText.isEmpty() || !headerLastText.isEmpty();
    bool hasFooter = !footerText.isEmpty() || !footerFirstText.isEmpty() || !footerLastText.isEmpty();

    write(startHtml);
    if (!headerFirstText.isEmpty()) {
        write(headerFirstText);
        write(separator);
    }
    if (!headerText.isEmpty()) {
        write(headerText);
        write(separator);
    } else if (hasFooter || hasHeader) {
        write(separator);
    }
    if (!headerLastText.isEmpty()) {
        write(headerLastText);
        write(separator);
    }
    write(contentText);
    if (!footerFirstText.isEmpty()) {
        write(separator);
        write(footerFirstText);
    }
    if (!footerText.isEmpty()) {
        write(separator);
        write(footerText);
    } else if (hasFooter || hasHeader) {
        write(separator);
    }
    if (!footerLastText.isEmpty()) {
        write(separator);
        write(footerLastText);
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

void PreviewTransformer::readHtml(QTextStream *headerFirst, QTextStream *header, QTextStream *headerLast,
                                  QTextStream *content,
                                  QTextStream *footerFirst, QTextStream *footer, QTextStream *footerLast) {
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
            readChangeContext(headerFirst, header, headerLast, content, footerFirst, footer, footerLast);
            continue;
        } else if (next == s5b) {
            readConditional();
            continue;
        }

        readComment();
    }
}

void PreviewTransformer::readChangeContext(QTextStream *headerFirst, QTextStream *header, QTextStream *headerLast,
                                           QTextStream *content,
                                           QTextStream *footerFirst, QTextStream *footer, QTextStream *footerLast) {
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

            currentStream = headerFirst;
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

            currentStream = headerLast;
        } else {
            currentStream = header;
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

            currentStream = footerFirst;
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

            currentStream = footerLast;
        } else {
            currentStream = footer;
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

