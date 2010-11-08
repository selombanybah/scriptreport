#include "textdocumentsyntaxhighlighter.h"

TextDocumentSyntaxHighlighter::TextDocumentSyntaxHighlighter(QObject *parent, OperationMode operationMode) :
    SyntaxHighlighter(parent, operationMode)
{
    init();
}

TextDocumentSyntaxHighlighter::TextDocumentSyntaxHighlighter(QTextDocument *parent, OperationMode operationMode) :
    SyntaxHighlighter(parent, operationMode)
{
    init();
}

TextDocumentSyntaxHighlighter::TextDocumentSyntaxHighlighter(QTextEdit *parent, OperationMode operationMode) :
    SyntaxHighlighter(parent, operationMode)
{
    init();
}

void TextDocumentSyntaxHighlighter::init() {

    QTextCharFormat xmlFormat;
    xmlFormat.setForeground(QBrush(QColor(192, 192, 192)));
    QTextCharFormat docTypeFormat;
    docTypeFormat.setForeground(QBrush(QColor(192, 192, 192)));
    QTextCharFormat entityFormat;
    entityFormat.setForeground(QBrush(QColor(128, 128, 128)));
    QTextCharFormat tagFormat;
    tagFormat.setForeground(QBrush(QColor(136,  18, 128)));
    m_validTagFormat = tagFormat;
    m_validTagFormat.setFontWeight(QFont::Bold);
    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::red);
//    commentFormat.setForeground(QBrush(QColor( 35, 110,  37)));
    QTextCharFormat commentMarkFormat = commentFormat;
    commentMarkFormat.setFontWeight(QFont::Bold);
    QTextCharFormat attributeNameFormat;
    attributeNameFormat.setForeground(QBrush(QColor(153,  69,   0)));
    m_validTagAttributteFormat = attributeNameFormat;
    m_validTagAttributteFormat.setFontWeight(QFont::Bold);
    QTextCharFormat attributeValueFormat;
    attributeValueFormat.setForeground(QBrush(QColor( 36,  36, 170)));
    QTextCharFormat srtFormat;
    srtFormat.setForeground(Qt::red);
//    srtFormat.setFontWeight(QFont::Bold);
    QTextCharFormat printSrtFormat;
    printSrtFormat.setForeground(Qt::blue);
//    printSrtFormat.setFontWeight(QFont::Bold);
    QTextCharFormat srtSectionFormat;
    srtSectionFormat.setForeground(QBrush(QColor(0, 218, 255)));
    m_srtValidSectionFormat = srtSectionFormat;
    m_srtValidSectionFormat.setFontWeight(QFont::Bold);

    m_formats[Html_XmlStart] = xmlFormat;
    m_formats[Html_DocTypeStart] = docTypeFormat;
    m_formats[Html_CommentStart] = commentFormat;

    m_formats[Html_XmlEnd] = xmlFormat;
    m_formats[Html_DocTypeEnd] = docTypeFormat;
    m_formats[Html_CommentEnd] = commentFormat;

    m_formats[Html_Xml] = xmlFormat;
    m_formats[Html_DocType] = docTypeFormat;
    m_formats[Html_Comment] = commentFormat;

    m_formats[Html_Entity] = entityFormat;

    m_formats[Html_AttributeName] = attributeNameFormat;

    m_formats[Html_AttributeValue] = attributeValueFormat;
    m_formats[Html_AttributeSingleQuoteValue] = attributeValueFormat;
    m_formats[Html_AttributeDoubleQuoteValue] = attributeValueFormat;

    m_formats[Html_TagEnd] = tagFormat;
    m_formats[Html_TagStart] = tagFormat;
    m_formats[Html_TagClose] = tagFormat;
    m_formats[Html_TagClose2] = tagFormat;
    m_formats[Html_TagName] = tagFormat;
    m_formats[Html_TagCloseName] = tagFormat;

    m_formats[JsCC_Begin1] = commentMarkFormat;
    m_formats[JsCC_Begin2] = commentMarkFormat;
    m_formats[JsCC_Begin3] = commentMarkFormat;
    m_formats[JsCC_Begin4] = commentMarkFormat;
    m_formats[JsCC_Begin5] = commentMarkFormat;
    m_formats[JsCC_Begin6] = commentMarkFormat;
    m_formats[JsCC_Begin7] = commentMarkFormat;
    m_formats[JsCC_Commnent1] = commentFormat;
    m_formats[JsCC_Commnent2] = commentFormat;
    m_formats[JsCC_Commnent3] = commentFormat;
    m_formats[JsCC_Commnent4] = commentFormat;
    m_formats[JsCC_Commnent5] = commentFormat;
    m_formats[JsCC_Commnent6] = commentFormat;
    m_formats[JsCC_Commnent7] = commentFormat;
    m_formats[JsCC_End1] = commentMarkFormat;
    m_formats[JsCC_End2] = commentMarkFormat;
    m_formats[JsCC_End3] = commentMarkFormat;
    m_formats[JsCC_End4] = commentMarkFormat;
    m_formats[JsCC_End5] = commentMarkFormat;
    m_formats[JsCC_End6] = commentMarkFormat;
    m_formats[JsCC_End7] = commentMarkFormat;

    m_formats[JsC_Begin1] = commentMarkFormat;
    m_formats[JsC_Begin2] = commentMarkFormat;
    m_formats[JsC_Begin3] = commentMarkFormat;
    m_formats[JsC_Begin4] = commentMarkFormat;
    m_formats[JsC_Begin5] = commentMarkFormat;
    m_formats[JsC_Begin6] = commentMarkFormat;
    m_formats[JsC_Begin7] = commentMarkFormat;
    m_formats[JsC_Commnent1] = commentFormat;
    m_formats[JsC_Commnent2] = commentFormat;
    m_formats[JsC_Commnent3] = commentFormat;
    m_formats[JsC_Commnent4] = commentFormat;
    m_formats[JsC_Commnent5] = commentFormat;
    m_formats[JsC_Commnent6] = commentFormat;
    m_formats[JsC_Commnent7] = commentFormat;
    m_formats[JsC_End1] = commentMarkFormat;
    m_formats[JsC_End2] = commentMarkFormat;
    m_formats[JsC_End3] = commentMarkFormat;
    m_formats[JsC_End4] = commentMarkFormat;
    m_formats[JsC_End5] = commentMarkFormat;
    m_formats[JsC_End6] = commentMarkFormat;
    m_formats[JsC_End7] = commentMarkFormat;

    m_formats[Srt_AttributeConditionalPrintStart] = printSrtFormat;
    m_formats[Srt_InlineConditionalPrintStart] = printSrtFormat;
    m_formats[Srt_InlineConditionalScriptStart] = srtFormat;
    m_formats[Srt_AttributePrintStart] = printSrtFormat;
    m_formats[Srt_InlinePrintStart] = printSrtFormat;
    m_formats[Srt_InlineScriptStart] = srtFormat;
    m_formats[Srt_ScriptStart] = srtFormat;
    m_formats[Srt_SectionStart] = srtSectionFormat;

    m_formats[Srt_AttributePrintEnd] = printSrtFormat;
    m_formats[Srt_AttributeConditionalPrintEnd] = printSrtFormat;
    m_formats[Srt_InlinePrintEnd] = printSrtFormat;
    m_formats[Srt_InlineConditionalPrintEnd] = printSrtFormat;
    m_formats[Srt_InlineConditionalScriptEnd] = srtFormat;
    m_formats[Srt_InlineScriptEnd] = srtFormat;
    m_formats[Srt_ScriptEnd] = srtFormat;
    m_formats[Srt_SectionEnd] = srtSectionFormat;

    m_formats[Srt_Section] = srtSectionFormat;
    m_formats[Srt_AttributeConditionalPrintElse] = printSrtFormat;
    m_formats[Srt_ConditionalPrintElse] = printSrtFormat;
    m_formats[Srt_ConditionalElse] = srtFormat;

//    m_formats[DocType]        = QColor(192, 192, 192);
//    m_formats[Entity]         = QColor(128, 128, 128);
//    m_formats[Tag]            = QColor(136,  18, 128);
//    m_formats[Comment]        = QColor( 35, 110,  37);
//    m_formats[AttributeName]  = QColor(153,  69,   0);
//    m_formats[AttributeValue] = QColor( 36,  36, 170);

    QTextCharFormat bgSrtFormat;
    bgSrtFormat.setBackground(QBrush(QColor(255, 242, 242)));
    QTextCharFormat bgPrintSrtFormat;
    bgPrintSrtFormat.setBackground(QBrush(QColor(242, 242, 255)));
    QTextCharFormat bgSrtSectionFormat;
    bgSrtSectionFormat.setBackground(QBrush(QColor(242, 253, 255)));

    m_backgrounds[Srt_AttributeConditionalPrintStart] = bgPrintSrtFormat;
    m_backgrounds[Srt_InlineConditionalPrintStart] = bgPrintSrtFormat;
    m_backgrounds[Srt_InlineConditionalScriptStart] = bgSrtFormat;
    m_backgrounds[Srt_AttributePrintStart] = bgPrintSrtFormat;
    m_backgrounds[Srt_InlinePrintStart] = bgPrintSrtFormat;
    m_backgrounds[Srt_InlineScriptStart] = bgSrtFormat;
    m_backgrounds[Srt_ScriptStart] = bgSrtFormat;
    m_backgrounds[Srt_SectionStart] = bgSrtSectionFormat;

    m_backgrounds[Srt_AttributePrintEnd] = bgPrintSrtFormat;
    m_backgrounds[Srt_AttributeConditionalPrintEnd] = bgPrintSrtFormat;
    m_backgrounds[Srt_InlinePrintEnd] = bgPrintSrtFormat;
    m_backgrounds[Srt_InlineConditionalPrintEnd] = bgPrintSrtFormat;
    m_backgrounds[Srt_InlineConditionalScriptEnd] = bgSrtFormat;
    m_backgrounds[Srt_InlineScriptEnd] = bgSrtFormat;
    m_backgrounds[Srt_ScriptEnd] = bgSrtFormat;
    m_backgrounds[Srt_SectionEnd] = bgSrtSectionFormat;

    m_backgrounds[Srt_AttributeConditionalPrintElse] = bgPrintSrtFormat;
    m_backgrounds[Srt_ConditionalPrintElse] = bgPrintSrtFormat;
    m_backgrounds[Srt_ConditionalElse] = bgSrtFormat;

    m_backgrounds[Srt_Section] = bgSrtSectionFormat;
    m_backgrounds[Srt_AttributeConditionalPrintElseText] = bgPrintSrtFormat;
    m_backgrounds[Srt_ConditionalPrintElseText] = bgPrintSrtFormat;
    m_backgrounds[Srt_ConditionalElseText] = bgSrtFormat;

    m_backgrounds[Js_AttributeConditionalPrintScript] = bgPrintSrtFormat;
    m_backgrounds[Js_InlineConditionalPrintScript] = bgPrintSrtFormat;
    m_backgrounds[Js_InlineConditionalScript] = bgSrtFormat;
    m_backgrounds[Js_AttributePrintScript] = bgPrintSrtFormat;
    m_backgrounds[Js_InlinePrintScript] = bgPrintSrtFormat;
    m_backgrounds[Js_InlineScript] = bgSrtFormat;
    m_backgrounds[Js_Script] = bgSrtFormat;

    m_backgrounds[JsCC_Begin1] = bgPrintSrtFormat;
    m_backgrounds[JsCC_Begin2] = bgPrintSrtFormat;
    m_backgrounds[JsCC_Begin3] = bgSrtFormat;
    m_backgrounds[JsCC_Begin4] = bgPrintSrtFormat;
    m_backgrounds[JsCC_Begin5] = bgPrintSrtFormat;
    m_backgrounds[JsCC_Begin6] = bgSrtFormat;
    m_backgrounds[JsCC_Begin7] = bgSrtFormat;
    m_backgrounds[JsCC_Commnent1] = bgPrintSrtFormat;
    m_backgrounds[JsCC_Commnent2] = bgPrintSrtFormat;
    m_backgrounds[JsCC_Commnent3] = bgSrtFormat;
    m_backgrounds[JsCC_Commnent4] = bgPrintSrtFormat;
    m_backgrounds[JsCC_Commnent5] = bgPrintSrtFormat;
    m_backgrounds[JsCC_Commnent6] = bgSrtFormat;
    m_backgrounds[JsCC_Commnent7] = bgSrtFormat;
    m_backgrounds[JsCC_End1] = bgPrintSrtFormat;
    m_backgrounds[JsCC_End2] = bgPrintSrtFormat;
    m_backgrounds[JsCC_End3] = bgSrtFormat;
    m_backgrounds[JsCC_End4] = bgPrintSrtFormat;
    m_backgrounds[JsCC_End5] = bgPrintSrtFormat;
    m_backgrounds[JsCC_End6] = bgSrtFormat;
    m_backgrounds[JsCC_End7] = bgSrtFormat;

    m_backgrounds[JsC_Begin1] = bgPrintSrtFormat;
    m_backgrounds[JsC_Begin2] = bgPrintSrtFormat;
    m_backgrounds[JsC_Begin3] = bgSrtFormat;
    m_backgrounds[JsC_Begin4] = bgPrintSrtFormat;
    m_backgrounds[JsC_Begin5] = bgPrintSrtFormat;
    m_backgrounds[JsC_Begin6] = bgSrtFormat;
    m_backgrounds[JsC_Begin7] = bgSrtFormat;
    m_backgrounds[JsC_Commnent1] = bgPrintSrtFormat;
    m_backgrounds[JsC_Commnent2] = bgPrintSrtFormat;
    m_backgrounds[JsC_Commnent3] = bgSrtFormat;
    m_backgrounds[JsC_Commnent4] = bgPrintSrtFormat;
    m_backgrounds[JsC_Commnent5] = bgPrintSrtFormat;
    m_backgrounds[JsC_Commnent6] = bgSrtFormat;
    m_backgrounds[JsC_Commnent7] = bgSrtFormat;
    m_backgrounds[JsC_End1] = bgPrintSrtFormat;
    m_backgrounds[JsC_End2] = bgPrintSrtFormat;
    m_backgrounds[JsC_End3] = bgSrtFormat;
    m_backgrounds[JsC_End4] = bgPrintSrtFormat;
    m_backgrounds[JsC_End5] = bgPrintSrtFormat;
    m_backgrounds[JsC_End6] = bgSrtFormat;
    m_backgrounds[JsC_End7] = bgSrtFormat;

    m_validTags << QString::fromLatin1("a")
                << QString::fromLatin1("address")
                << QString::fromLatin1("b")
                << QString::fromLatin1("big")
                << QString::fromLatin1("blockquote")
                << QString::fromLatin1("body")
                << QString::fromLatin1("br")
                << QString::fromLatin1("center")
                << QString::fromLatin1("cite")
                << QString::fromLatin1("code")
                << QString::fromLatin1("dd")
                << QString::fromLatin1("dfn")
                << QString::fromLatin1("div")
                << QString::fromLatin1("dl")
                << QString::fromLatin1("dt")
                << QString::fromLatin1("em")
                << QString::fromLatin1("font")
                << QString::fromLatin1("h1")
                << QString::fromLatin1("h2")
                << QString::fromLatin1("h3")
                << QString::fromLatin1("h4")
                << QString::fromLatin1("h5")
                << QString::fromLatin1("h6")
                << QString::fromLatin1("head")
                << QString::fromLatin1("hr")
                << QString::fromLatin1("html")
                << QString::fromLatin1("i")
                << QString::fromLatin1("img")
                << QString::fromLatin1("kbd")
                << QString::fromLatin1("meta")
                << QString::fromLatin1("li")
                << QString::fromLatin1("nobr")
                << QString::fromLatin1("ol")
                << QString::fromLatin1("p")
                << QString::fromLatin1("pre")
                << QString::fromLatin1("qt")
                << QString::fromLatin1("s")
                << QString::fromLatin1("samp")
                << QString::fromLatin1("small")
                << QString::fromLatin1("span")
                << QString::fromLatin1("strong")
                << QString::fromLatin1("sub")
                << QString::fromLatin1("sup")
                << QString::fromLatin1("table")
                << QString::fromLatin1("tbody")
                << QString::fromLatin1("td")
                << QString::fromLatin1("tfoot")
                << QString::fromLatin1("th")
                << QString::fromLatin1("thead")
                << QString::fromLatin1("title")
                << QString::fromLatin1("tr")
                << QString::fromLatin1("tt")
                << QString::fromLatin1("u")
                << QString::fromLatin1("ul")
                << QString::fromLatin1("var");
    m_validTags.squeeze();

    QSet<QString> blockAttributes;
    blockAttributes << QString::fromLatin1("align")
                    << QString::fromLatin1("dir");
    blockAttributes.squeeze();

    QSet<QString> listAttributes;
    listAttributes << QString::fromLatin1("type");
    listAttributes.squeeze();

    QSet<QString> tableCellAttributes;
    tableCellAttributes << QString::fromLatin1("width")
                        << QString::fromLatin1("bgcolor")
                        << QString::fromLatin1("colspan")
                        << QString::fromLatin1("rowspan")
                        << QString::fromLatin1("align")
                        << QString::fromLatin1("valign");
    tableCellAttributes.squeeze();

    QSet<QString> anchorAttributes;
    anchorAttributes << QString::fromLatin1("href")
                     << QString::fromLatin1("name");
    anchorAttributes.squeeze();

    QSet<QString> bodyAttributes;
    bodyAttributes << QString::fromLatin1("bgcolor");
    bodyAttributes.squeeze();

    QSet<QString> fontAttributes;
    fontAttributes << QString::fromLatin1("size")
                   << QString::fromLatin1("face")
                   << QString::fromLatin1("color");
    fontAttributes.squeeze();

    QSet<QString> hrAttributes;
    hrAttributes << QString::fromLatin1("width");
    hrAttributes.squeeze();

    QSet<QString> imgAttibutes;
    imgAttibutes << QString::fromLatin1("src")
                 << QString::fromLatin1("source")
                 << QString::fromLatin1("width")
                 << QString::fromLatin1("height");
    imgAttibutes.squeeze();

    QSet<QString> metaAttibutes;
    metaAttibutes << QString::fromLatin1("http-equiv")
                  << QString::fromLatin1("content");
    metaAttibutes.squeeze();

    QSet<QString> tableAttibutes;
    tableAttibutes << QString::fromLatin1("border")
                   << QString::fromLatin1("bgcolor")
                   << QString::fromLatin1("cellspacing")
                   << QString::fromLatin1("cellpadding")
                   << QString::fromLatin1("width")
                   << QString::fromLatin1("height");
    tableAttibutes.squeeze();

    QSet<QString> trAttributes;
    trAttributes << QString::fromLatin1("bgcolor");
    trAttributes.squeeze();

    m_tagAttributes[QString::fromLatin1("a")] = anchorAttributes;
    m_tagAttributes[QString::fromLatin1("body")] = bodyAttributes;
    m_tagAttributes[QString::fromLatin1("div")] = blockAttributes;
    m_tagAttributes[QString::fromLatin1("dl")] = blockAttributes;
    m_tagAttributes[QString::fromLatin1("dt")] = blockAttributes;
    m_tagAttributes[QString::fromLatin1("font")] = fontAttributes;
    m_tagAttributes[QString::fromLatin1("h1")] = blockAttributes;
    m_tagAttributes[QString::fromLatin1("h2")] = blockAttributes;
    m_tagAttributes[QString::fromLatin1("h3")] = blockAttributes;
    m_tagAttributes[QString::fromLatin1("h4")] = blockAttributes;
    m_tagAttributes[QString::fromLatin1("h5")] = blockAttributes;
    m_tagAttributes[QString::fromLatin1("h6")] = blockAttributes;
    m_tagAttributes[QString::fromLatin1("hr")] = hrAttributes;
    m_tagAttributes[QString::fromLatin1("img")] = imgAttibutes;
    m_tagAttributes[QString::fromLatin1("meta")] = metaAttibutes;
    m_tagAttributes[QString::fromLatin1("ol")] = listAttributes;
    m_tagAttributes[QString::fromLatin1("p")] = blockAttributes;
    m_tagAttributes[QString::fromLatin1("table")] = tableAttibutes;
    m_tagAttributes[QString::fromLatin1("td")] = tableCellAttributes;
    m_tagAttributes[QString::fromLatin1("th")] = tableCellAttributes;
    m_tagAttributes[QString::fromLatin1("tr")] = trAttributes;
    m_tagAttributes[QString::fromLatin1("ul")] = listAttributes;
    m_tagAttributes.squeeze();

    m_tagStatus << QString::fromLatin1("a")
                << QString::fromLatin1("body")
                << QString::fromLatin1("div")
                << QString::fromLatin1("dl")
                << QString::fromLatin1("dt")
                << QString::fromLatin1("font")
                << QString::fromLatin1("h1")
                << QString::fromLatin1("h2")
                << QString::fromLatin1("h3")
                << QString::fromLatin1("h4")
                << QString::fromLatin1("h5")
                << QString::fromLatin1("h6")
                << QString::fromLatin1("hr")
                << QString::fromLatin1("img")
                << QString::fromLatin1("meta")
                << QString::fromLatin1("ol")
                << QString::fromLatin1("p")
                << QString::fromLatin1("table")
                << QString::fromLatin1("td")
                << QString::fromLatin1("th")
                << QString::fromLatin1("tr")
                << QString::fromLatin1("ul");

    m_jsKeywordPatterns << QRegExp(QString::fromLatin1("\\bif\\b"))
                        << QRegExp(QString::fromLatin1("\\bbreak\\b"))
                        << QRegExp(QString::fromLatin1("\\bdo\\b"))
                        << QRegExp(QString::fromLatin1("\\binstanceof\\b"))
                        << QRegExp(QString::fromLatin1("\\btypeof\\b"))
                        << QRegExp(QString::fromLatin1("\\bcase\\b"))
                        << QRegExp(QString::fromLatin1("\\belse\\b"))
                        << QRegExp(QString::fromLatin1("\\bnew\\b"))
                        << QRegExp(QString::fromLatin1("\\bvar\\b"))
                        << QRegExp(QString::fromLatin1("\\bcatch\\b"))
                        << QRegExp(QString::fromLatin1("\\bfinally\\b"))
                        << QRegExp(QString::fromLatin1("\\breturn\\b"))
                        << QRegExp(QString::fromLatin1("\\bvoid\\b"))
                        << QRegExp(QString::fromLatin1("\\bcontinue\\b"))
                        << QRegExp(QString::fromLatin1("\\bfor\\b"))
                        << QRegExp(QString::fromLatin1("\\bswitch\\b"))
                        << QRegExp(QString::fromLatin1("\\bwhile\\b"))
                        << QRegExp(QString::fromLatin1("\\bdebugger\\b"))
                        << QRegExp(QString::fromLatin1("\\bfunction\\b"))
                        << QRegExp(QString::fromLatin1("\\bthis\\b"))
                        << QRegExp(QString::fromLatin1("\\bwith\\b"))
                        << QRegExp(QString::fromLatin1("\\bdefault\\b"))
                        << QRegExp(QString::fromLatin1("\\bthrow\\b"))
                        << QRegExp(QString::fromLatin1("\\bdelete\\b"))
                        << QRegExp(QString::fromLatin1("\\bin\\b"))
                        << QRegExp(QString::fromLatin1("\\btry\\b"))
                        << QRegExp(QString::fromLatin1("\\bclass\\b"))
                        << QRegExp(QString::fromLatin1("\\benum\\b"))
                        << QRegExp(QString::fromLatin1("\\bextends\\b"))
                        << QRegExp(QString::fromLatin1("\\bsuper\\b"))
                        << QRegExp(QString::fromLatin1("\\bconst\\b"))
                        << QRegExp(QString::fromLatin1("\\bexport\\b"))
                        << QRegExp(QString::fromLatin1("\\bimport\\b"))
                        << QRegExp(QString::fromLatin1("\\bimplements\\b"))
                        << QRegExp(QString::fromLatin1("\\blet\\b"))
                        << QRegExp(QString::fromLatin1("\\bprivate\\b"))
                        << QRegExp(QString::fromLatin1("\\bpublic\\b"))
                        << QRegExp(QString::fromLatin1("\\binterface\\b"))
                        << QRegExp(QString::fromLatin1("\\bpackage\\b"))
                        << QRegExp(QString::fromLatin1("\\bprotected\\b"))
                        << QRegExp(QString::fromLatin1("\\bstatic\\b"))
                        << QRegExp(QString::fromLatin1("\\byield\\b"));

    m_jsKeywordFormat.setForeground(Qt::darkYellow);
    m_jsKeywordFormat.setFontWeight(QFont::Bold);

    m_cssKeywordPatterns << QRegExp(QString::fromLatin1("\\bbackground-color\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bbackground-image\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bcolor\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bfont-family\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bfont-size\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bfont-style\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bfont-weight\\s*:"))
                         << QRegExp(QString::fromLatin1("\\btext-decoration\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bfont\\s*:"))
                         << QRegExp(QString::fromLatin1("\\btext-indent\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bwhite-space\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bmargin-top\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bmargin-bottom\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bmargin-left\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bmargin-right\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bpadding-top\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bpadding-bottom\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bpadding-left\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bpadding-right\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bpadding\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bvertical-align\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bborder-color\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bborder-style\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bbackground\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bpage-break-before\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bpage-break-after\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bfloat\\s*:"))
                         << QRegExp(QString::fromLatin1("\\btext-transform\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bfont-variant\\s*:"))
                         << QRegExp(QString::fromLatin1("\\bword-spacing\\s*:"));

    m_cssKeywordFormat = attributeValueFormat;
    m_cssKeywordFormat.setFontWeight(QFont::Bold);

}

void TextDocumentSyntaxHighlighter::process(const QString &text, Token tokens[], int &customState, bool &reprocessFutureToken) {
    SyntaxHighlighter::process(text, tokens, customState, reprocessFutureToken);

    Token token = tokens[CurrentToken];
    if (token.isMark) {
        return;
    }

    QTextCharFormat format = m_formats.value(token.type);
    QTextCharFormat background;
    if (operationMode() == ScriptReportTemplateMode) {
        background = m_backgrounds.value(token.type);
    }

    QString tokenText = text.mid(token.start, token.end - token.start + 1);
    QString lowerTokenText = tokenText.toLower();

    if (token.type == Srt_Section) {
        if (tokenText == QString::fromLatin1("header")
                || tokenText == QString::fromLatin1("headerFirst")
                || tokenText == QString::fromLatin1("headerLast")
                || tokenText == QString::fromLatin1("content")
                || tokenText == QString::fromLatin1("footer")
                || tokenText == QString::fromLatin1("footerFirst")
                || tokenText == QString::fromLatin1("footerLast")) {
            format = m_srtValidSectionFormat;
        }
    } else if (token.type == Html_TagName) {
        if( m_validTags.contains(lowerTokenText)) {
            format = m_validTagFormat;
        }
        tokens[LastTagNameToken].custom = m_tagStatus.indexOf(lowerTokenText);
    } else if(token.type == Html_TagCloseName) {
        if( m_validTags.contains(lowerTokenText)) {
            format = m_validTagFormat;
        }
    } else if (token.type == Html_AttributeName) {
        if (lowerTokenText == QString::fromLatin1("style")) {
            format = m_validTagAttributteFormat;
            tokens[LastAttributeNameToken].custom = 0;
        } else {
            int currentTag = tokens[LastTagNameToken].custom;
            if (currentTag >= 0) {
                QSet<QString> attributes = m_tagAttributes[m_tagStatus[currentTag]];
                if (attributes.contains(lowerTokenText)) {
                    format = m_validTagAttributteFormat;
                }
            }
        }
    }

    if (format.isValid()) {
        if (background.isValid()) {
            format.setBackground(background.background());
        }
    } else {
        format = background;
    }

    if (format.isValid()) {
        setFormat(token.start, token.end - token.start + 1, format);
    }

    if (token.type > JsBegin && token.type < JsEnd) {
        foreach (QRegExp expression, m_jsKeywordPatterns) {
            int index = expression.indexIn(tokenText);
            while (index >= 0) {
                int length = expression.matchedLength();
                QTextCharFormat keywordFormat = m_jsKeywordFormat;
                if (format.isValid()) {
                    keywordFormat.setBackground(format.background());
                }
                setFormat(token.start + index, length, keywordFormat);
                index = expression.indexIn(tokenText, index + length);
            }
        }

        QRegExp number(QString::fromLatin1("(-?\\d+(\\.\\d*)?)|(-?\\.\\d+)"));
        int index = number.indexIn(tokenText);
        while (index >= 0) {
            int length = number.matchedLength();
            QTextCharFormat numberFormat;
            numberFormat.setForeground(Qt::blue);
            if (format.isValid()) {
                numberFormat.setBackground(format.background());
            }
            setFormat(token.start + index, length, numberFormat);
            index = number.indexIn(tokenText, index + length);
        }

        QRegExp quotation(QString::fromLatin1("\\'([^\\']|\\\\')*(\\'|$)"));
        index = quotation.indexIn(tokenText);
        while (index >= 0) {
            int length = quotation.matchedLength();
            QTextCharFormat quotationFormat;
            quotationFormat.setForeground(Qt::darkGreen);
            if (format.isValid()) {
                quotationFormat.setBackground(format.background());
            }
            setFormat(token.start + index, length, quotationFormat);
            index = quotation.indexIn(tokenText, index + length);
        }

        QRegExp doubleQuotation(QString::fromLatin1("\\\"([^\\\"]|\\\\\")*(\\\"|$)"));
        index = doubleQuotation.indexIn(tokenText);
        while (index >= 0) {
            int length = doubleQuotation.matchedLength();
            QTextCharFormat doubleQuotationFormat;
            doubleQuotationFormat.setForeground(Qt::darkGreen);
            if (format.isValid()) {
                doubleQuotationFormat.setBackground(format.background());
            }
            setFormat(token.start + index, length, doubleQuotationFormat);
            index = doubleQuotation.indexIn(tokenText, index + length);
        }

    } else if (tokens[LastAttributeNameToken].custom == 0) {
        if (token.type == Html_AttributeValue || token.type == Html_AttributeSingleQuoteValue ||
                token.type == Html_AttributeDoubleQuoteValue) {

            foreach (QRegExp expression, m_cssKeywordPatterns) {
                int index = expression.indexIn(tokenText);
                while (index >= 0) {
                    int length = expression.matchedLength();
                    QTextCharFormat keywordFormat = m_cssKeywordFormat;
                    if (format.isValid()) {
                        keywordFormat.setBackground(format.background());
                    }
                    setFormat(token.start + index, length, keywordFormat);
                    index = expression.indexIn(tokenText, index + length);
                }
            } // end foreach

        }
    }

}

void TextDocumentSyntaxHighlighter::initHighlighting(const QString &/*text*/, const int previousState, Token tokens[], int &/*customState*/) {
    if (previousState == -1) {
        tokens[FutureToken].type = inialState();
    } else {
        int lastAttributte = previousState >> 14;
        int lastTag = (previousState >> 8) - (lastAttributte << 6) ;
        int lastToken = previousState - ( (previousState >> 8) << 8);

        tokens[FutureToken].type = (TokenType) lastToken;
        tokens[LastTagNameToken].custom = lastTag - 1;
        tokens[LastAttributeNameToken].custom = lastAttributte - 1;
    }
}

int TextDocumentSyntaxHighlighter::getFinalState(const QString &/*text*/, Token tokens[], int /*customState*/) {
    int lastAttributte = tokens[LastAttributeNameToken].custom + 1;
    int lastTag = tokens[LastTagNameToken].custom + 1;
    int lastToken = (int) tokens[CurrentToken].type;

    int finalState = (lastAttributte << 14) + (lastTag << 8) + lastToken;
    return finalState;
}
