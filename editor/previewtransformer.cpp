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

    if (!in || !out) {
        return false;
    }

    write(startHtml);
    consume();
    if (next.isNull()) {
        write(endHtml);
        out->flush();
        return true;
    }
    consume();
    if (next.isNull()) {
        *out << current;
        write(endHtml);
        out->flush();
        return true;
    }
    readHtml();
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
    *out << current;
    consume();
}

void PreviewTransformer::write(const QChar &c) {
    *out << c;
}

void PreviewTransformer::write(const QChar &c1, const QChar &c2) {
    *out << c1 << c2;
}

void PreviewTransformer::write(const QString &s) {
    *out << s;
}

void PreviewTransformer::readHtml() {
    const QChar s1  = QChar::fromLatin1('<');
    const QChar s2  = QChar::fromLatin1('!');
    const QChar s3  = QChar::fromLatin1('-');
    const QChar s4  = QChar::fromLatin1('-');
    const QChar s5a = QChar::fromLatin1(':');

    while (!current.isNull()) {
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
            readChangeContext();
            continue;
        }

        readComment();
    }
}

void PreviewTransformer::readChangeContext() {
    // content
    const QChar c1 = QChar::fromLatin1('c');
    const QChar c2 = QChar::fromLatin1('o');
    const QChar c3 = QChar::fromLatin1('n');
    const QChar c4 = QChar::fromLatin1('t');
    const QChar c5 = QChar::fromLatin1('e');
    const QChar c6 = QChar::fromLatin1('n');
    const QChar c7 = QChar::fromLatin1('t');
    const QString content = QString::fromLatin1("<hr>");

    // footer
    const QChar f1 = QChar::fromLatin1('f');
    const QChar f2 = QChar::fromLatin1('o');
    const QChar f3 = QChar::fromLatin1('o');
    const QChar f4 = QChar::fromLatin1('t');
    const QChar f5 = QChar::fromLatin1('e');
    const QChar f6 = QChar::fromLatin1('r');
    const QString footer = QString::fromLatin1("<hr>");

    prepare();
    if (current == c1) {
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
