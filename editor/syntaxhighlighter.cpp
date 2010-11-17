#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QObject *parent, OperationMode operationMode)
    : QSyntaxHighlighter(parent)
{
    m_operationMode = operationMode;
    init();
}

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent, OperationMode operationMode)
    : QSyntaxHighlighter(parent)
{
    m_operationMode = operationMode;
    init();
}

SyntaxHighlighter::SyntaxHighlighter(QTextEdit *parent, OperationMode operationMode)
    : QSyntaxHighlighter(parent)
{
    m_operationMode = operationMode;
    init();
}

void SyntaxHighlighter::init() {
}

static inline char at(QString t, int pos) {
    return t.at(pos).toAscii();
}

void SyntaxHighlighter::highlightBlock(const QString &text)
{
    Token tokens[TokenElementSize];

    Token &token = tokens[FutureToken];
    Token &previousToken = tokens[CurrentToken];
    Token &tagNameToken = tokens[LastTagNameToken];
    Token &attributeNameToken = tokens[LastAttributeNameToken];

    token.start = -1;
    token.end = -1;
    token.isMark = true;
    token.type = Invalid;
    token.custom = -1;

    previousToken = token;
    tagNameToken = token;
    attributeNameToken = token;

    int customState = -1;
    int previousState = previousBlockState();
//    qWarning(qPrintable(QString::fromLatin1("Previous state: `%1'").arg(previousState)));
    initHighlighting(text, previousState, tokens, customState);

    // Note: added the \n character to the text

    QString t = text + QString::fromLatin1("\nXXXXXXXXXX"); // prevent index out of range in lookaheads

    int len = text.length() + 1;
    int pos = 0;
    int countDown = 2 * len + 4; // prevent infinity bucle if exists a problem

    while (--countDown > 0) {
        if (!previousToken.isMark &&
                !token.isMark &&
                previousToken.type == token.type) {
            // Extend the previous token
            previousToken.end = token.end;
        } else {
            bool reprocessFutureToken = false;
            process(text, tokens, customState, reprocessFutureToken);
            if (reprocessFutureToken) {
             token = previousToken;
             pos = token.end + 1;
            } else {
                previousToken = token;
            }
        }

        if (pos <= token.end) {
            pos = token.end + 1;
        }
        if (pos >= len) {
            if (pos == len) {
                // introduce a mark previous to the break
                token.isMark = true;
                pos++;
                continue;
            }
            break;
        }

        char ch = at(t,pos);

        token.start = pos;
        token.isMark = false;

        if (token.type == Html_Text) { // <<<<<<<<<<<<<<<<<<<<<<<<<<Html_Text<<<<<<<<<<<<<<

            if (ch == '<') { // ************************************Html_Text*********** <
                char ch1 = at(t,pos + 1);
                if (ch1 == '?') { // *******************************Html_Text********* <?
                    // <? in <?xml ... >
                    token.end = pos + 1;
                    token.type = Html_XmlStart;
                } else if (ch1 == '!' ) { // ***********************Html_Text********* <!
                    char ch2 = at(t,pos + 2);
                    if (ch2 == '-' ) { // **************************Html_Text******* <!-
                        char ch3 = at(t,pos + 3);
                        if (ch3 == '-' ) { // **********************Html_Text***** <!--
                            if (m_operationMode == ScriptReportTemplateMode) {
                                char ch4 = at(t,pos + 4);
                                if (ch4 == '@' ) { // **************Html_Text*** <!--@
                                    // <!--@
                                    token.end = pos + 4;
                                    token.type = Srt_ScriptStart;
                                } else if (ch4 == '$' ) { // *******Html_Text*** <!--$
                                    // <!--$
                                    token.end = pos + 4;
                                    token.type = Srt_InlineScriptStart;
                                } else if (ch4 == '?' ) { // *******Html_Text*** <!--?
                                    // <!--?
                                    token.end = pos + 4;
                                    token.type = Srt_InlineConditionalScriptStart;
                                } else if (ch4 == ':' ) { // *******Html_Text*** <!--:
                                    // <!--:
                                    token.end = pos + 4;
                                    token.type = Srt_SectionStart;
                                } else { // ************************Html_Text**** <!--
                                    // <!--
                                    token.end = pos + 3;
                                    token.type = Html_Comment;
                                } // end html comment
                            } else { // ****************************Html_Text**** <!--
                                // <!--
                                token.end = pos + 3;
                                token.type = Html_Comment;
                            } // end html comment
                        } // end <!--
                    } else { // end <!- // *************************Html_Text******** <!
                        // <! in <!DOCTYPE ... >
                        token.end = pos + 1;
                        token.type = Html_DocType;
                    } // end <! in <!DOCTYPE ... >
                } else { // end <! // ******************************Html_Text********** <
                    // <
                    token.end = pos;
                    token.type = Html_TagStart;
                }
                continue;
            } // end <

            if (m_operationMode == ScriptReportTemplateMode) {
                if (ch == '$') { // ********************************Html_Text*********** $
                    char ch1 = at(t,pos + 1);
                    if (ch1 == '{') { // ***************************Html_Text********* ${
                        token.end = pos + 1;
                        token.type = Srt_InlinePrintStart;
                        continue;
                    }
                }

                if (ch == '?') { // ********************************Html_Text*********** ?
                    char ch1 = at(t,pos + 1);
                    if (ch1 == '{') { // ***************************Html_Text********* ?{
                        token.end = pos + 1;
                        token.type = Srt_InlineConditionalPrintStart;
                        continue;
                    }
                }
            }

            if (ch == '&') { // ************************************Html_Text*********** &
                int entityEnd = pos;
                while (entityEnd < len
                       && at(t,++entityEnd) != ';') {
                    ; // ignore
                }
                token.end = entityEnd;
                token.type = Html_Entity;
                continue;
            }

            // consume
            token.end = pos;
            continue;
        } // end Html_Text



        if (token.type > HtmlBegin &&
            token.type < HtmlEnd) { // <<<<<<<<<<<<<<<<<<<<<<<Html(Begin-End)<<<<<<<<<<<<<<

            if (m_operationMode == ScriptReportTemplateMode) {
                if (token.type == Html_AttributeValue ||
                        token.type == Html_AttributeSingleQuoteValue ||
                        token.type == Html_AttributeDoubleQuoteValue) {
                    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Html_AttributeValue<<<<<<<<<<<<<
                    if (ch == '$') { // ******************Html_AttributeValue*********** $
                        char ch1 = at(t,pos + 1);
                        if (ch1 == '{') { // *************Html_AttributeValue********* ${
                            token.end = pos + 1;
                            token.type = Srt_AttributePrintStart;
                            continue;
                        }
                    }
                    if (ch == '?') { // *****************Html_AttributeValue*********** ?
                        char ch1 = at(t,pos + 1);
                        if (ch1 == '{') { // ************Html_AttributeValue********* ?{
                            token.end = pos + 1;
                            token.type = Srt_AttributeConditionalPrintStart;
                            continue;
                        }
                    }
                } // end Html_AttributeValue
            }

            if (ch == '-' && token.type == Html_Comment) { // ***Html_Comment*********** -
                char ch1 = at(t,pos + 1);
                if (ch1 == '-') { // ****************************Html_Comment********* --
                    char ch2 = at(t,pos + 2);
                    if (ch2 == '>') { // ************************Html_Comment******* -->
                        token.end = pos + 2;
                        token.type = Html_CommentEnd;
                        continue;
                    }
                }
            }

            // escape char secuence
            if (ch == '\\') { // ******************************************************* \ .
                char ch1 = at(t,pos + 1);
                if (ch1 == '\'') { // ************************************************ \'
                    if (token.type == Html_AttributeSingleQuoteValue) {
                        // ********************Html_AttributeSingleQuoteValue******** \'
                        token.end = pos + 1;
                        continue;
                    }
                }
                if (ch1 == '"') { // ************************************************* \"
                    if (token.type == Html_AttributeDoubleQuoteValue) {
                        // ********************Html_AttributeDoubleQuoteValue******** \"
                        token.end = pos + 1;
                        continue;
                    }
                }
            } // end \ .

            if (ch == '>') { // ******************************************************** >
                if (token.type == Html_Xml) { // ********************Html_Xml********** >
                    token.end = pos;
                    token.type = Html_XmlEnd;
                    continue;
                }
                if (token.type == Html_DocType) { // ************Html_DocType********** >
                    token.end = pos;
                    token.type = Html_DocTypeEnd;
                    continue;
                }
                if (token.type == Html_AttributeReady ||
                        token.type == Html_TagClose2 ||
                        token.type == Html_TagEndReady ||
                        token.type == Html_TagCloseName) {
                    // *********************************Html_AttributeReady/
                    //                    Html_Tag(Close2/EndReady/CloseName)********** >
                    token.end = pos;
                    token.type = Html_TagEnd;
                    continue;
                }
                if (token.type == Html_TagName) {
                    // ******************************************Html_TagName********** >
                    token.end = pos;
                    token.type = Html_TagEnd;
                    tagNameToken = previousToken;
                    continue;
                }

            } // end >

            if (ch == '/') { // ******************************************************** /
                if (token.type == Html_TagStart ||
                        token.type == Html_TagNameReady) {
                    // *****************************Html_Tag(Start/NameReady)********** /
                    token.end = pos;
                    token.type = Html_TagClose;
                    continue;
                }
                if (token.type == Html_AttributeReady ||
                        token.type == Html_TagName) {
                    // *************************Html_(AttributeReady/TagName)********** /
                    token.end = pos;
                    token.type = Html_TagClose2;
                    continue;
                }
            } // end /

            if (ch == '=') { // ******************************************************** =
                if (token.type == Html_AttributeName) {
                    // ************************************Html_AttributeName********** =
                    token.end = pos;
                    token.type = Html_AttributeAssignment;
                    attributeNameToken = previousToken;
                    continue;
                }
                if (token.type == Html_AttributeAssignmentReady) {
                    // *************************Html_AttributeAssignmentReady********** =
                    token.end = pos;
                    token.type = Html_AttributeAssignment;
                    continue;
                }
            }


            if ( ch == '\'') { // ****************************************************** '
                if (token.type == Html_AttributeAssignment ||
                        token.type == Html_AttributeValueReady) {
                    // *****************Html_Attribute(Assignment/ValueReady)********** '
                    token.end = pos;
                    token.type = Html_AttributeSingeQuoteStart;
                    continue;
                }
                if (token.type == Html_AttributeSingleQuoteValue) {
                    // ************************Html_AttributeSingleQuoteValue********** '
                    token.end = pos;
                    token.type = Html_AttributeSingleQuoteEnd;
                    continue;
                }
            } // end '

            if (ch == '"') { // ******************************************************** "
                if (token.type == Html_AttributeAssignment ||
                        token.type == Html_AttributeValueReady) {
                    // *****************Html_Attribute(Assignment/ValueReady)********** "
                    token.end = pos;
                    token.type = Html_AttributeDoubleQuoteStart;
                    continue;
                }
                if (token.type == Html_AttributeDoubleQuoteValue) {
                    // ************************Html_AttributeDoubleQuoteValue********** "
                    token.end = pos;
                    token.type = Html_AttributeDoubleQuoteEnd;
                    continue;
                }
            } // end "

            if (ch == ' ' || ch == '\r' || ch == '\n') { // *************************** sp
                if (token.type == Html_AttributeValue ||
                        token.type == Html_AttributeReady) {
                    // ***************************Html_Attribute(Value/Ready)********* sp
                    token.end = pos;
                    token.type = Html_AttributeReady;
                    token.isMark = true;
                    continue;
                }
                if (token.type == Html_TagName) {
                    // ******************************************Html_TagName********* sp
                    token.end = pos;
                    token.type = Html_AttributeReady;
                    token.isMark = true;
                    tagNameToken = previousToken;
                    continue;
                }
                if (token.type == Html_AttributeName) {
                    // ************************************Html_AttributeName********* sp
                    token.end = pos;
                    token.type = Html_AttributeAssignmentReady;
                    token.isMark = true;
                    attributeNameToken = previousToken;
                    continue;
                }
                if (token.type == Html_AttributeAssignmentReady) {
                    // *************************Html_AttributeAssignmentReady********* sp
                    token.end = pos;
                    token.type = Html_AttributeAssignmentReady;
                    token.isMark = true;
                    continue;
                }
                if (token.type == Html_TagCloseName) {
                    // *************************************Html_TagCloseName********* sp
                    token.end = pos;
                    token.type = Html_AttributeAssignmentReady;
                    token.isMark = true;
                    continue;
                }
                if (token.type > HtmlPreSecuencialReadyBegin &&
                        token.type < HtmlPreSecuencialReadyEnd) {
                    // *********************HtmlPreSecuencialReady(Begin-End)********* sp
                    token.end = pos;
                    int value = (int)(token.type - HtmlPreSecuencialReadyBegin + HtmlSecuencialReadyBegin);
                    token.type = (TokenType) value;
                    continue;
                }
                if (token.type > HtmlSecuencialReadyBegin &&
                        token.type < HtmlSecuencialReadyEnd) {
                    // *********************HtmlPreSecuencialReady(Begin-End)********* sp
                    token.end = pos;
                    continue;
                }
            } // end sp

            if (token.type > HtmlContentBegin && token.type < HtmlContentEnd) {
                token.end = pos;
                continue;
            }

            if (token.type > HtmlStartElementBegin && token.type < HtmlStartElementEnd) {
                token.end = pos - 1;
                int value = (int)(token.type - HtmlStartElementBegin + HtmlContentElementBegin);
                token.type = (TokenType) value;
                token.isMark = true;
                continue;
            }

            ////// begin

            if (token.type == Html_AttributeReady || token.type == Html_AttributeAssignmentReady) {
                token.end = pos - 1;
                token.type = Html_AttributeName;
                token.isMark = true;
                continue;
            }

            if (token.type == Html_TagStart || token.type == Html_TagNameReady) {
                token.end = pos - 1;
                token.type = Html_TagName;
                token.isMark = true;
                continue;
            }

            if (token.type == Html_TagClose || token.type == Html_TagCloseNameReady) {
                token.end = pos - 1;
                token.type = Html_TagCloseName;
                token.isMark = true;
                continue;
            }

            if (token.type == Html_AttributeAssignment || token.type == Html_AttributeValueReady) {
                token.end = pos - 1;
                token.type = Html_AttributeValue;
                token.isMark = true;
                continue;
            }

            ////// end

            if (token.type == Html_AttributeSingeQuoteStart) {
                token.end = pos - 1;
                token.type = Html_AttributeSingleQuoteValue;
                token.isMark = true;
                continue;
            }

            if (token.type == Html_AttributeDoubleQuoteStart) {
                token.end = pos - 1;
                token.type = Html_AttributeDoubleQuoteValue;
                token.isMark = true;
                continue;
            }

            if (token.type > HtmlToHtmlTextBegin &&
                    token.type < HtmlToHtmlTextEnd) {
                token.end = pos - 1;
                token.type = Html_Text;
                token.isMark = true;
                continue;
            }

            if (token.type == Html_AttributeSingleQuoteEnd ||
                    token.type == Html_AttributeDoubleQuoteEnd) {
                token.end = pos - 1;
                token.type = Html_AttributeReady;
                token.isMark = true;
                continue;
            }

            // ignore
            pos++;
            token.isMark = true;
        } // end Html(Begin-End)



        if (token.type > SrtBegin &&
            token.type < SrtEnd) { // <<<<<<<<<<<<<<<<<<<<<<<<<Srt(Begin-End)<<<<<<<<<<<<<<

            if (ch == '-') { // ******************************************************** -
                char ch1 = at(t,pos + 1);
                if (ch1 == '-') { // ************************************************* --
                    char ch2 = at(t,pos + 2);
                    if (ch2 == '>') { // ******************************************* -->
                        if (token.type == Srt_Section) { // ******Srt_Section****** -->
                            token.end = pos + 2;
                            token.type = Srt_SectionEnd;
                            continue;
                        }
                        if (token.type == Srt_ConditionalElseText) {
                            // ***********************Srt_ConditionalElseText****** -->
                            token.end = pos + 2;
                            token.type = Srt_InlineConditionalScriptEnd;
                            continue;
                        }
                    }
                }
            }

            if ( ch == '}') { // ******************************************************* }
                if (token.type == Srt_ConditionalPrintElseText) {
                    // *************************Srt_ConditionalPrintElseText*********** }
                    token.end = pos;
                    token.type = Srt_InlineConditionalPrintEnd;
                    continue;
                }
                if (token.type == Srt_AttributeConditionalPrintElseText) {
                    // ****************Srt_AttributeConditionalPrintElseText*********** }
                    token.end = pos;
                    token.type = Srt_AttributeConditionalPrintEnd;
                    continue;
                }
            }

            if (token.type == Srt_Section) {
                token.end = pos;
                continue;
            }

            if (token.type > SrtConditionalTextBegin && token.type < SrtConditionalTextEnd) {
                token.end = pos;
                continue;
            }

            if (token.type == Srt_SectionStart) {
                token.end = pos - 1;
                token.type = Srt_Section;
                token.isMark = true;
                continue;
            }

            if (token.type > SrtCondionalElseBegin &&
                    token.type < SrtConditionalElseEnd) {
                token.end = pos - 1;
                int value = (int)(token.type - SrtCondionalElseBegin + SrtConditionalTextBegin);
                token.type = (TokenType) value;
                token.isMark = true;
                continue;
            }

            if (token.type > SrtStartJsBegin &&
                    token.type < SrtStartJsEnd) {
                token.end = pos - 1;
                int value = (int)(token.type - SrtStartJsBegin + JsBegin);
                token.type = (TokenType) value;
                token.isMark = true;
                continue;
            }

            if (token.type > SrtEndJsBegin_NoAttribute &&
                    token.type < SrtEndJsEnd) {
                token.end = pos - 1;
                token.type = Html_Text;
                token.isMark = true;
                continue;
            }

            if (token.type == Srt_SectionEnd) {
                token.end = pos - 1;
                token.type = Html_Text;
                token.isMark = true;
                continue;
            }

            if (token.type == Srt_AttributePrintEnd ||
                    token.type == Srt_AttributeConditionalPrintEnd) {
                token.end = pos - 1;
                token.type = Html_AttributeValue;
                token.isMark = true;
                continue;
            }

            // ignore
            pos++;
            token.isMark = true;
        } // end Srt(Begin-End)

        if (token.type > JsBegin &&
            token.type < JsEnd) { // <<<<<<<<<<<<<<<<<<<<<<<<<<<Js(Begin-End)<<<<<<<<<<<<<<

            if (m_operationMode == ScriptReportTemplateMode) {
                if (ch == '-') { // **************************************************** -
                    char ch1 = at(t,pos + 1);
                    if (ch1 == '-') { // ********************************************* --
                        char ch2 = at(t,pos + 2);
                        if (ch2 == '>') { // *************************************** -->
                            if (token.type == Js_Script) { // *****Js_Script******* -->
                                token.end = pos + 2;
                                token.type = Srt_ScriptEnd;
                                continue;
                            }
                            if (token.type == Js_InlineScript) {
                                // **************************Js_InlineScript******* -->
                                token.end = pos + 2;
                                token.type = Srt_InlineScriptEnd;
                                continue;
                            }
                            if (token.type == Js_InlineConditionalScript) {
                                // ***************Js_InlineConditionalScript******* -->
                                token.end = pos + 2;
                                token.type = Srt_InlineConditionalScriptEnd;
                                continue;
                            }
                        }
                    }
                } // end -->

                if (ch == ':') { // **************************************************** :
                    char ch1 = at(t,pos + 1);
                    if (ch1 == ':') { // ********************************************** ::
                        if (token.type == Js_InlineConditionalScript) {
                            // *******************Js_InlineConditionalScript********* ::
                            token.end = pos + 1;
                            token.type = Srt_ConditionalElse;
                            continue;
                        }
                        if (token.type == Js_InlineConditionalPrintScript) {
                            // **************Js_InlineConditionalPrintScript********* ::
                            token.end = pos + 1;
                            token.type = Srt_ConditionalPrintElse;
                            continue;
                        }
                        if (token.type == Js_AttributeConditionalPrintScript) {
                            // ***********Js_AttributeConditionalPrintScript********* ::
                            token.end = pos + 1;
                            token.type = Srt_AttributeConditionalPrintElse;
                            continue;
                        }
                    }
                } // end ::

                if (ch == '}') { // **************************************************** }
                    if (token.type == Js_InlinePrintScript) {
                        // *****************************Js_InlinePrintScript*********** }
                        token.end = pos;
                        token.type = Srt_InlinePrintEnd;
                        continue;
                    }
                    if (token.type == Js_InlineConditionalPrintScript) {
                        // ******************Js_InlineConditionalPrintScript*********** }
                        token.end = pos;
                        token.type = Srt_InlineConditionalPrintEnd;
                        continue;
                    }
                    if (token.type == Js_AttributePrintScript) {
                        // **************************Js_AttributePrintScript*********** }
                        token.end = pos;
                        token.type = Srt_AttributePrintEnd;
                        continue;
                    }
                    if (token.type == Js_AttributeConditionalPrintScript) {
                        // ***************Js_AttributeConditionalPrintScript*********** }
                        token.end = pos;
                        token.type = Srt_AttributeConditionalPrintEnd;
                        continue;
                    }
                } // end }
            }

            if (ch == '/') { // ******************************************************** /
                char ch1 = at(t,pos + 1);
                if (ch1 == '*') { // ************************************************* /*
                    if (token.type > JsBegin && token.type < JsEnd) {
                        // ************************************Js(Begin-End)********* /*
                        token.end = pos + 1;
                        int value = (int)(token.type - JsBegin + JsCC_Begin1) - 1;
                        token.type = (TokenType) value;
                        continue;
                    }
                }
                if (ch1 == '/') { // ************************************************* //
                    if (token.type > JsBegin && token.type < JsEnd) {
                        // ************************************Js(Begin-End)********* //
                        token.end = pos + 1;
                        int value = (int)(token.type - JsBegin + JsC_Begin1) - 1;
                        token.type = (TokenType) value;
                        continue;
                    }
                }
            } // end /

            // consume
            token.end = pos;
            continue;
        } // end Js(Begin-End)



        if (token.type > JsCCommentBegin &&
            token.type < JsCCommentEnd) { // <<<<<<<<<JsCComment(Begin-End)<<<<<<<<<<<<<<

            if (ch == '*' && token.type >= JsCC_Commnent1 &&
                    token.type <= JsCC_Commnent7) { // **********Js(Begin-End)*********** *
                char ch1 = at(t,pos + 1);
                if (ch1 == '/') { // ***************************Js(Begin-End)********* */
                    token.end = pos + 1;
                    int value = (int)(token.type - JsCC_Commnent1 + JsCC_End1);
                    token.type = (TokenType) value;
                    continue;
                }
            }

            if (token.type >= JsCC_Commnent1 && token.type <= JsCC_Commnent7) {
                token.end = pos;
                continue;
            }

            if (token.type >= JsCC_Begin1 &&
                    token.type <= JsCC_Begin7) {
                token.end = pos - 1;
                int value = (int)(token.type - JsCC_Begin1 + JsCC_Commnent1);
                token.type = (TokenType) value;
                token.isMark = true;
                continue;
            }

            if (token.type >= JsCC_End1 &&
                    token.type <= JsCC_End7) {
                token.end = pos - 1;
                int value = (int)(token.type - JsCC_End1 + JsBegin) + 1;
                token.type = (TokenType) value;
                token.isMark = true;
                continue;
            }

            // ignore
            pos++;
            token.isMark = true;
        } // end JsCComment(Begin-End)



        if (token.type > JsCommentBegin &&
            token.type < JsCommentEnd) { // <<<<<<<<<<<JsComment(Begin-End)<<<<<<<<<<<<<<

            if ((ch == '\r' || ch == '\n') && token.type >= JsC_Commnent1 &&
                    token.type <= JsC_Commnent7) { // ****JsCC_Commnent(1-7)********** \n
                token.end = pos;
                int value = (int)(token.type - JsC_Commnent1 + JsC_End1);
                token.type = (TokenType) value;
                continue;
            }

            if (ch == '}') { // ******************************************************** }
                if (token.type == JsC_Commnent1) {
                    // ****************************************JsC_Commnent1*********** }
                    token.end = pos - 1;
                    token.type = JsC_End1;
                    token.isMark = true;
                    continue;
                }
                if (token.type == JsC_Commnent2) {
                    // ****************************************JsC_Commnent2*********** }
                    token.end = pos - 1;
                    token.type = JsC_End2;
                    token.isMark = true;
                    continue;
                }
                if (token.type == JsC_Commnent4) {
                    // ****************************************JsC_Commnent4*********** }
                    token.end = pos - 1;
                    token.type = JsC_End4;
                    token.isMark = true;
                    continue;
                }
                if (token.type == JsC_Commnent5) {
                    // ****************************************JsC_Commnent5*********** }
                    token.end = pos - 1;
                    token.type = JsC_End5;
                    token.isMark = true;
                    continue;
                }
            } // end }

            if (token.type >= JsC_Commnent1 && token.type <= JsC_Commnent7) {
                token.end = pos;
                continue;
            }

            if (token.type >= JsC_Begin1 &&
                    token.type <= JsC_Begin7) {
                token.end = pos - 1;
                int value = (int)(token.type - JsC_Begin1 + JsC_Commnent1);
                token.type = (TokenType) value;
                token.isMark = true;
                continue;
            }

            if (token.type >= JsC_End1 &&
                    token.type <= JsC_End7) {
                token.end = pos - 1;
                int value = (int)(token.type - JsC_End1 + JsBegin) + 1;
                token.type = (TokenType) value;
                token.isMark = true;
                continue;
            }

            // ignore
            pos++;
            token.isMark = true;
        } // end JsComment(Begin-End)

        // ignore
        pos++;
        token.isMark = true;
    }

    setCurrentBlockState(getFinalState(text, tokens, customState));
}

SyntaxHighlighter::OperationMode SyntaxHighlighter::operationMode() {
    return m_operationMode;
}

void SyntaxHighlighter::setOperationMode(OperationMode operationMode) {
    m_operationMode = operationMode;
}

/*
 * Uncomment for test propouse
 */

//static inline void printToken(const QString &text, const SyntaxHighlighter::Token &token, const char *name) {
//    QString result = QString::fromLatin1("Token %1 start: `%2', end: `%3', type: `%4', isMark: `%5', text: `%6'")
//                     .arg(QString::fromLatin1(name))
//                     .arg(token.start)
//                     .arg(token.end)
//                     .arg(token.type)
//                     .arg(token.isMark)
//                     .arg(text.mid(token.start, token.end - token.start + 1));
//    qWarning(qPrintable(result));
//}
//
//void SyntaxHighlighter::process(const QString &text, Token tokens[], int &customState, bool &reprocessFutureToken) {
//    Token token = tokens[CurrentToken];
//
//    if (token.isMark) {
//        return;
//    }
//
//    if (token.type == Html_AttributeValue ||
//            token.type == Html_AttributeSingleQuoteValue ||
//            token.type == Html_AttributeDoubleQuoteValue) {
//        printToken(text, tokens[LastTagNameToken], "tag name");
//        printToken(text, tokens[LastAttributeNameToken], "attribute name");
//        printToken(text, token, "attribute value");
//    } else if (token.type == Html_Text) {
//        printToken(text, tokens[LastTagNameToken], "tag name");
//        printToken(text, token, "text");
//    } else {
//        printToken(text, token, "value");
//    }
//}

void SyntaxHighlighter::process(const QString &/*text*/, Token /*tokens*/[], int &/*customState*/, bool &/*reprocessFutureToken*/) {
}

void SyntaxHighlighter::initHighlighting(const QString &/*text*/, const int previousState, Token tokens[], int &/*customState*/) {
    if (previousState == -1) {
        tokens[FutureToken].type = inialState();
    } else {
        tokens[FutureToken].type = (TokenType) previousState;
    }
}

int SyntaxHighlighter::getFinalState(const QString &/*text*/, Token tokens[], int /*customState*/) {
    return (int) tokens[CurrentToken].type;
}

SyntaxHighlighter::TokenType SyntaxHighlighter::inialState() {
    if (m_operationMode == JavaScriptMode) {
        return Js_Script;
    } else {
        return Html_Text;
    }
}
