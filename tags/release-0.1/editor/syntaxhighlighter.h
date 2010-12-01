#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QtCore/QHash>
#include <QtGui/QSyntaxHighlighter>

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    enum OperationMode {
        HtmlMode,
        JavaScriptMode,
        ScriptReportTemplateMode
    };

    enum TokenType {
        Invalid,
        Html_Text,              //+ html text                   | <= none

/*i*/   HtmlBegin,

/*j*/   HtmlStartElementBegin,
        Html_XmlStart,          //. <? in <?xml... >            | <= Html_Text
        Html_DocTypeStart,      //. <! in <!DOCTYPE... >        | <= Html_Text
        Html_CommentStart,      //. <!-- in <!-- comment -->    | <= Html_Text
/*/j*/  HtmlStartElementEnd,

/*n*/   HtmlToHtmlTextBegin,

/*l*/   HtmlEndElementBegin,
        Html_XmlEnd,            //. > in <?xml... >             | <= Html_Xml
                                //                                => Html_Text
        Html_DocTypeEnd,        //. > in <!DOCTYPE... >         | <= Html_DocType
                                //                                => Html_Text
        Html_CommentEnd,        //. --> in <!-- comment -->     | <= Html_Comment
                                //                                => Html_Text
/*/l*/  HtmlEndElementEnd,

        Html_Entity,            //. &amp;                       | <= Html_Text
                                //                                => Html_Text

        Html_TagEnd,            //. > in <br>                   | <= Html_AttributeReady ||
                                //                                   Html_TagClose2      ||
                                //                                   Html_TagEndReady    ||
                                //                                   Html_TagName        ||
                                //                                   Html_TagCloseName   ||
                                //                                   Html_AttributeName  ||
                                //                                   Html_AttributeValue
                                //                                => Html_Text
/*/n*/  HtmlToHtmlTextEnd,

        Html_AttributeSingeQuoteStart,  //. first ' in <table height='5'>  | <= Html_AttributeAssignment ||
                                        //                                      Html_AttributeValueReady
        Html_AttributeDoubleQuoteStart, //. first " in <table height="5">  | <= Html_AttributeAssignment ||
                                        //                                      Html_AttributeValueReady
        Html_AttributeSingleQuoteEnd,   //. second ' in <table height='5'> | <= Html_AttributeSingleQuoteValue
                                        //                                   => Html_AttributeReady
        Html_AttributeDoubleQuoteEnd,   //. second " in <table height="5"> | <= Html_AttributeDoubleQuoteValue
                                        //                                   => Html_AttributeReady


/*o*/   HtmlPreSecuencialReadyBegin,
        Html_TagStart,                  //. < in <br>                      | <= Html_Text
        Html_TagClose,                  //. / in </table>                  | <= Html_TagStart       ||
                                        //                                      Html_TagNameReady
        Html_TagClose2,                 //. / in <br />                    | <= Html_AttributeReady ||
                                        //                                      Html_TagName
        Html_AttributeAssignment,       //. = in <table height="5">        | <= Html_AttributeName  ||
                                        //                                      Html_AttributeAssignmentReady
/*/o*/  HtmlPreSecuencialReadyEnd,

/*p*/   HtmlSecuencialReadyBegin,
        Html_TagNameReady,              //. mark for read tag name         | <= (Html_TagStart || Html_TagNameReady) && sp
        Html_TagCloseNameReady,         //. mark for read tag close name   | <= (Html_TagClose || Html_TagCloseNameReady) && sp
        Html_TagEndReady,               //. mark for read tag end          | <= (Html_TagClose2 || Html_TagEndReady) && sp ||
                                        //                                      Html_TagCloseName && sp
        Html_AttributeValueReady,       //. mark for read attribute value  | <= (Html_AttributeAssignment || Html_AttributeValueReady) && sp
/*/p*/  HtmlSecuencialReadyEnd,

        Html_AttributeAssignmentReady,  //. mark for read = in attribute   | <= (Html_AttributeName || Html_AttributeAssignmentReady) && sp
        Html_AttributeReady,            //. mark for read attribute        | <= (Html_TagName || Html_AttributeReady) && sp ||
                                        //                                      (Html_AttributeName || Html_AttributeValue) && sp

/*q*/   HtmlContentBegin,
        Html_AttributeName,             //+ height in <table height="5">   | <= Html_AttributeReady      ||
                                        //                                      Html_AttributeAssignmentReady
        Html_TagName,                   //+ table in <table>               | <= Html_TagStart            ||
                                        //                                      Html_TagNameReady
        Html_TagCloseName,              //+ table in </table>              | <= Html_TagClose            ||
                                        //                                      Html_TagCloseNameReady
        Html_AttributeValue,            //+ 5 in <table height=5>          | <= Html_AttributeAssignment ||
                                        //                                      Html_AttributeValueReady
        Html_AttributeSingleQuoteValue, //+ 5 in <table height='5'>        | <= Html_AttributeSingeQuoteStart
        Html_AttributeDoubleQuoteValue, //+ 5 in <table height="5">        | <= Html_AttributeDoubleQuoteStart

/*k*/   HtmlContentElementBegin,
        Html_Xml,                       //+ xml... in <?xml... >           | <= Html_XmlStart
        Html_DocType,                   //+ DOCTYPE... in <!DOCTYPE...>    | <= Html_DocTypeStart
        Html_Comment,                   //+ commnent in <!--comment-->     | <= Html_CommentStart
/*/k*/  HtmlContentElementEnd,
/*/q*/  HtmlContentEnd,

/*/i*/  HtmlEnd,



/*a*/   SrtBegin,

/*b*/   SrtStartJsBegin,
        Srt_AttributeConditionalPrintStart,    //. ?{ in attribute value        | <= Html_AttributeValue
        Srt_AttributeSingleConditionalPrintStart, //. ?{ in attribute value     | <= Html_AttributeValue
        Srt_AttributeDoubleConditionalPrintStart, //. ?{ in attribute value     | <= Html_AttributeValue
        Srt_InlineConditionalPrintStart,       //. ?{                           | <= Html_Text
        Srt_InlineConditionalScriptStart,      //. <!--?                        | <= Html_Text
        Srt_AttributePrintStart,               //. ${ in attribute value        | <= Html_AttributeValue
        Srt_AttributeSinglePrintStart,         //. ${ in attribute value        | <= Html_AttributeValue
        Srt_AttributeDoublePrintStart,         //. ${ in attribute value        | <= Html_AttributeValue
        Srt_InlinePrintStart,                  //. ${                           | <= Html_Text
        Srt_InlineScriptStart,                 //. <!--$                        | <= Html_Text
        Srt_ScriptStart,                       //. <!--@                        | <= Html_Text
/*/b*/  SrtStartJsEnd,

        Srt_SectionStart,                      //. <!--:                        | <= Html_Text
        Srt_Section,                           //+ section in <!--:section-->   | <= Srt_SectionStart
        Srt_SectionEnd,                        //. -->                          | <= Srt_Section
                                               //                                 => Html_Text

/*g*/   SrtCondionalElseBegin,
        Srt_AttributeConditionalPrintElse,     //. :: in attribute ?{js::text}  | <= Js_AttributeConditionalPrintScript
        Srt_AttributeSingleConditionalPrintElse,//. :: in attribute ?{js::text} | <= Js_AttributeSingleConditionalPrintScript
        Srt_AttributeDoubleConditionalPrintElse,//. :: in attribute ?{js::text} | <= Js_AttributeDoubleConditionalPrintScript
        Srt_ConditionalPrintElse,              //. :: in ?{ js :: text }        | <= Js_InlineConditionalPrintScript
        Srt_ConditionalElse,                   //. :: in <!--?js::text -->      | <= Js_InlineConditionalScript
/*/g*/  SrtConditionalElseEnd,
/*h*/   SrtConditionalTextBegin,
        Srt_AttributeConditionalPrintElseText, //+ text in attribute ?{js::text}| <= Srt_AttributeConditionalPrintElse
        Srt_AttributeSingleConditionalPrintElseText,//+ text in attribute ?{js::text}| <= Srt_AttributeSingleConditionalPrintElse
        Srt_AttributeDoubleConditionalPrintElseText,//+ text in attribute ?{js::text}| <= Srt_AttributeDoubleConditionalPrintElse
        Srt_ConditionalPrintElseText,          //+ text in ?{ js :: text }      | <= Srt_ConditionalPrintElse
        Srt_ConditionalElseText,               //+ text in <!--?js::text -->    | <= Srt_ConditionalElse
/*/h*/  SrtConditionalTextEnd,

/*d*/   SrtEndJsBegin,
        Srt_AttributePrintEnd,                 //. } in attribute value         | <= Js_AttributePrintScript
                                               //                                 => Html_AttributeValue
        Srt_AttributeSinglePrintEnd,           //. } in attribute value         | <= Js_AttributeSinglePrintScript
                                               //                                 => Html_AttributeSingleQuoteValue
        Srt_AttributeDoublePrintEnd,           //. } in attribute value         | <= Js_AttributeDoublePrintScript
                                               //                                 => Html_AttributeDoubleQuoteValue
        Srt_AttributeConditionalPrintEnd,      //. } in attribute value         | <= Js_AttributeConditionalPrintScript ||
                                               //                                    Srt_AttributeConditionalPrintElseText
                                               //                                 => Html_AttributeValue
        Srt_AttributeSingleConditionalPrintEnd,//. } in attribute value         | <= Js_AttributeSingleConditionalPrintScript ||
                                               //                                    Srt_AttributeSingleConditionalPrintElseText
                                               //                                 => Html_AttributeSingleQuoteValue
        Srt_AttributeDoubleConditionalPrintEnd,//. } in attribute value         | <= Js_AttributeDoubleConditionalPrintScript ||
                                               //                                    Srt_AttributeDoubleConditionalPrintElseText
                                               //                                 => Html_AttributeDoubleQuoteValue
/*d'*/  SrtEndJsBegin_NoAttribute,
        Srt_InlinePrintEnd,                    //. }                            | <= Js_InlinePrintScript
                                               //                                 => Html_Text
        Srt_InlineConditionalPrintEnd,         //. }                            | <= Js_InlineConditionalPrintScript    ||
                                               //                                    Srt_ConditionalPrintElseText
                                               //                                 => Html_Text
        Srt_InlineConditionalScriptEnd,        //. -->                          | <= Js_InlineConditionalScript         ||
                                               //                                    Srt_ConditionalElseText
                                               //                                 => Html_Text
        Srt_InlineScriptEnd,                   //. -->                          | <= Js_InlineScript
                                               //                                 => Html_Text
        Srt_ScriptEnd,                         //. -->                          | <= Js_Script
                                               //                                 => Html_Text
/*/d*/  SrtEndJsEnd,

/*/a*/  SrtEnd,



        JsBegin,
 /*1*/  Js_AttributeConditionalPrintScript, //+ js in ?{ js } as attribute value| <= Srt_AttributeConditionalPrintStart
 /*8*/  Js_AttributeSingleConditionalPrintScript, //+ js in ?{ js } as attribute value| <= Srt_AttributeSinglrConditionalPrintStart
 /*9*/  Js_AttributeDoubleConditionalPrintScript, //+ js in ?{ js } as attribute value| <= Srt_AttributeDoubleConditionalPrintStart
 /*2*/  Js_InlineConditionalPrintScript,    //+ js in ?{ js }                   | <= Srt_InlineConditionalPrintStart
 /*3*/  Js_InlineConditionalScript,         //+ js in <!--? js -->              | <= Srt_InlineConditionalScriptStart
 /*4*/  Js_AttributePrintScript,            //+ js in ${ js } as attribute value| <= Srt_AttributePrintStart
 /*10*/ Js_AttributeSinglePrintScript,      //+ js in ${ js } as attribute value| <= Srt_AttributeSinglePrintStart
 /*11*/ Js_AttributeDoublePrintScript,      //+ js in ${ js } as attribute value| <= Srt_AttributeDoublePrintStart
 /*5*/  Js_InlinePrintScript,               //+ js in ${ js }                   | <= Srt_InlinePrintStart
 /*6*/  Js_InlineScript,                    //+ js in <!--$ js -->              | <= Srt_InlineScriptStart
 /*7*/  Js_Script,                          //+ js in <!--@ js -->              | <= Srt_ScriptStart
        JsEnd,



/*e*/   JsCCommentBegin,
        JsCC_Begin1,    //. /* in /* commnet */ for /*1*/      | <= Js_AttributeConditionalPrintScript
        JsCC_Begin8,    //. /* in /* commnet */ for /*8*/      | <= Js_AttributeSingleConditionalPrintScript
        JsCC_Begin9,    //. /* in /* commnet */ for /*9*/      | <= Js_AttributeDoubleConditionalPrintScript
        JsCC_Begin2,    //. /* in /* commnet */ for /*2*/      | <= Js_InlineConditionalPrintScript
        JsCC_Begin3,    //. /* in /* commnet */ for /*3*/      | <= Js_InlineConditionalScript
        JsCC_Begin4,    //. /* in /* commnet */ for /*4*/      | <= Js_AttributePrintScript
        JsCC_Begin10,   //. /* in /* commnet */ for /*10*/     | <= Js_AttributeSinglePrintScript
        JsCC_Begin11,   //. /* in /* commnet */ for /*11*/     | <= Js_AttributeDoublePrintScript
        JsCC_Begin5,    //. /* in /* commnet */ for /*5*/      | <= Js_InlinePrintScript
        JsCC_Begin6,    //. /* in /* commnet */ for /*6*/      | <= Js_InlineScript
        JsCC_Begin7,    //. /* in /* commnet */ for /*7*/      | <= Js_Script
        JsCC_Commnent1, //+ comment in /* comment */ for /*1*/ | <= JsCC_Begin1
        JsCC_Commnent8, //+ comment in /* comment */ for /*8*/ | <= JsCC_Begin8
        JsCC_Commnent9, //+ comment in /* comment */ for /*9*/ | <= JsCC_Begin9
        JsCC_Commnent2, //+ comment in /* comment */ for /*2*/ | <= JsCC_Begin2
        JsCC_Commnent3, //+ comment in /* comment */ for /*3*/ | <= JsCC_Begin3
        JsCC_Commnent4, //+ comment in /* comment */ for /*4*/ | <= JsCC_Begin4
        JsCC_Commnent10,//+ comment in /* comment */ for /*10*/ | <= JsCC_Begin10
        JsCC_Commnent11,//+ comment in /* comment */ for /*11*/ | <= JsCC_Begin11
        JsCC_Commnent5, //+ comment in /* comment */ for /*5*/ | <= JsCC_Begin5
        JsCC_Commnent6, //+ comment in /* comment */ for /*6*/ | <= JsCC_Begin6
        JsCC_Commnent7, //+ comment in /* comment */ for /*7*/ | <= JsCC_Begin7
        JsCC_End1,      //. */ in /* comment */ for /*1*/      | <= JsCC_Commnent1
                        //                                       => Js_AttributeConditionalPrintScript
        JsCC_End8,      //. */ in /* comment */ for /*8*/      | <= JsCC_Commnent8
                        //                                       => Js_AttributeSingleConditionalPrintScript
        JsCC_End9,      //. */ in /* comment */ for /*9*/      | <= JsCC_Commnent9
                        //                                       => Js_AttributeDoubleConditionalPrintScript
        JsCC_End2,      //. */ in /* comment */ for /*2*/      | <= JsCC_Commnent2
                        //                                       => Js_InlineConditionalPrintScript
        JsCC_End3,      //. */ in /* comment */ for /*3*/      | <= JsCC_Commnent3
                        //                                       => Js_InlineConditionalScript
        JsCC_End4,      //. */ in /* comment */ for /*4*/      | <= JsCC_Commnent4
                        //                                       => Js_AttributePrintScript
        JsCC_End10,     //. */ in /* comment */ for /*10*/     | <= JsCC_Commnent10
                        //                                       => Js_AttributeSinglePrintScript
        JsCC_End11,     //. */ in /* comment */ for /*11*/     | <= JsCC_Commnent11
                        //                                       => Js_AttributeDoublePrintScript
        JsCC_End5,      //. */ in /* comment */ for /*5*/      | <= JsCC_Commnent5
                        //                                       => Js_InlinePrintScript
        JsCC_End6,      //. */ in /* comment */ for /*6*/      | <= JsCC_Commnent6
                        //                                       => Js_InlineScript
        JsCC_End7,      //. */ in /* comment */ for /*7*/      | <= JsCC_Commnent7
                        //                                       => Js_Script
/*/e*/  JsCCommentEnd,



/*f*/   JsCommentBegin,
        JsC_Begin1,    //. // in // commnet for /*1*/            | <= Js_AttributeConditionalPrintScript
        JsC_Begin8,    //. // in // commnet for /*8*/            | <= Js_AttributeSingleConditionalPrintScript
        JsC_Begin9,    //. // in // commnet for /*9*/            | <= Js_AttributeDoubleConditionalPrintScript
        JsC_Begin2,    //. // in // commnet for /*2*/            | <= Js_InlineConditionalPrintScript
        JsC_Begin3,    //. // in // commnet for /*3*/            | <= Js_InlineConditionalScript
        JsC_Begin4,    //. // in // commnet for /*4*/            | <= Js_AttributePrintScript
        JsC_Begin10,   //. // in // commnet for /*10*/           | <= Js_AttributeSinglePrintScript
        JsC_Begin11,   //. // in // commnet for /*11*/           | <= Js_AttributeDoublePrintScript
        JsC_Begin5,    //. // in // commnet for /*5*/            | <= Js_InlinePrintScript
        JsC_Begin6,    //. // in // commnet for /*6*/            | <= Js_InlineScript
        JsC_Begin7,    //. // in // commnet for /*7*/            | <= Js_Script
        JsC_Commnent1, //+ comment in // comment for /*1*/       | <= JsC_Begin1
        JsC_Commnent8, //+ comment in // comment for /*8*/       | <= JsC_Begin8
        JsC_Commnent9, //+ comment in // comment for /*9*/       | <= JsC_Begin9
        JsC_Commnent2, //+ comment in // comment for /*2*/       | <= JsC_Begin2
        JsC_Commnent3, //+ comment in // comment for /*3*/       | <= JsC_Begin3
        JsC_Commnent4, //+ comment in // comment for /*4*/       | <= JsC_Begin4
        JsC_Commnent10,//+ comment in // comment for /*10*/      | <= JsC_Begin10
        JsC_Commnent11,//+ comment in // comment for /*11*/      | <= JsC_Begin11
        JsC_Commnent5, //+ comment in // comment for /*5*/       | <= JsC_Begin5
        JsC_Commnent6, //+ comment in // comment for /*6*/       | <= JsC_Begin6
        JsC_Commnent7, //+ comment in // comment for /*7*/       | <= JsC_Begin7
        JsC_End1,      //. end line or } in // comment for /*1*/ | <= JsC_Commnent1
                       //                                          => Js_AttributeConditionalPrintScript
        JsC_End8,      //. end line or } in // comment for /*8*/ | <= JsC_Commnent8
                       //                                          => Js_AttributeSingleConditionalPrintScript
        JsC_End9,      //. end line or } in // comment for /*9*/ | <= JsC_Commnent9
                       //                                          => Js_AttributeDoubleConditionalPrintScript
        JsC_End2,      //. end line or } in // comment for /*2*/ | <= JsC_Commnent2
                       //                       i                   => Js_InlineConditionalPrintScript
        JsC_End3,      //. end line in // comment for /*3*/      | <= JsC_Commnent3
                       //                                          => Js_InlineConditionalScript
        JsC_End4,      //. end line or } in // comment for /*4*/ | <= JsC_Commnent4
                       //                                          => Js_AttributePrintScript
        JsC_End10,     //. end line or } in // comment for /*10*/| <= JsC_Commnent10
                       //                                          => Js_AttributeSinglePrintScript
        JsC_End11,     //. end line or } in // comment for /*11*/| <= JsC_Commnent11
                       //                                          => Js_AttributeDoublePrintScript
        JsC_End5,      //. end line or } in // comment for /*5*/ | <= JsC_Commnent5
                       //                                          => Js_InlinePrintScript
        JsC_End6,      //. end line in // comment for /*6*/      | <= JsC_Commnent6
                       //                                          => Js_InlineScript
        JsC_End7,      //. end line in // comment for /*7*/      | <= JsC_Commnent7
                       //                                          => Js_Script
/*/f*/  JsCommentEnd,

        Custom = 1000

    };

    enum TokenElement {
        CurrentToken,
        FutureToken,
        LastTagNameToken,
        LastAttributeNameToken,
        TokenElementSize
    };

    struct Token {
        int start;
        int end;
        TokenType type;
        bool isMark;
        int custom;
    };


    explicit SyntaxHighlighter(QObject *parent, OperationMode operationMode = ScriptReportTemplateMode);
    explicit SyntaxHighlighter(QTextDocument *parent, OperationMode operationMode = ScriptReportTemplateMode);
    explicit SyntaxHighlighter(QTextEdit *parent, OperationMode operationMode = ScriptReportTemplateMode);

    OperationMode operationMode();
    void setOperationMode(OperationMode operationMode);

protected:
    void highlightBlock(const QString &text);
    TokenType inialState();

    virtual void process(const QString &text, Token tokens[], int &customState, bool &reprocessFutureToken);
    virtual void initHighlighting(const QString &text, const int previousState, Token tokens[], int &customState);
    virtual int getFinalState(const QString &text, Token tokens[], int customState);


private:
    void init();

private:
    OperationMode m_operationMode;
};



















/*
enum all {
    Invalid,
    Html_Text,
    HtmlBegin,
    HtmlStartElementBegin,
    Html_XmlStart,
    Html_DocTypeStart,
    Html_CommentStart,
    HtmlStartElementEnd,
    HtmlToHtmlTextBegin,
    HtmlEndElementBegin,
    Html_XmlEnd,
    Html_DocTypeEnd,
    Html_CommentEnd,
    HtmlEndElementEnd,
    Html_Entity,
    Html_TagEnd,
    HtmlToHtmlTextEnd,
    Html_AttributeSingeQuoteStart,
    Html_AttributeDoubleQuoteStart,
    Html_AttributeSingleQuoteEnd,
    Html_AttributeDoubleQuoteEnd,
    HtmlPreSecuencialReadyBegin,
    Html_TagStart,
    Html_TagClose,
    Html_TagClose2,
    Html_AttributeAssignment,
    HtmlPreSecuencialReadyEnd,
    HtmlSecuencialReadyBegin,
    Html_TagNameReady,
    Html_TagCloseNameReady,
    Html_TagEndReady,
    Html_AttributeValueReady,
    HtmlSecuencialReadyEnd,
    Html_AttributeAssignmentReady,
    Html_AttributeReady,
    HtmlContentBegin,
    Html_AttributeName,
    Html_TagName,
    Html_TagCloseName,
    Html_AttributeValue,
    Html_AttributeSingleQuoteValue,
    Html_AttributeDoubleQuoteValue,
    HtmlContentElementBegin,
    Html_Xml,
    Html_DocType,
    Html_Comment,
    HtmlContentElementEnd,
    HtmlContentEnd,
    HtmlEnd,
    SrtBegin,
    SrtStartJsBegin,
    Srt_AttributeConditionalPrintStart,
    Srt_InlineConditionalPrintStart,
    Srt_InlineConditionalScriptStart,
    Srt_AttributePrintStart,
    Srt_InlinePrintStart,
    Srt_InlineScriptStart,
    Srt_ScriptStart,
    SrtStartJsEnd,
    Srt_SectionStart,
    Srt_Section,
    Srt_SectionEnd,
    SrtCondionalElseBegin,
    Srt_AttributeConditionalPrintElse,
    Srt_ConditionalPrintElse,
    Srt_ConditionalElse,
    SrtConditionalElseEnd,
    SrtConditionalTextBegin,
    Srt_AttributeConditionalPrintElseText,
    Srt_ConditionalPrintElseText,
    Srt_ConditionalElseText,
    SrtConditionalTextEnd,
    SrtEndJsBegin,
    Srt_AttributePrintEnd,
    Srt_AttributeConditionalPrintEnd,
    SrtEndJsBegin_NoAttribute,
    Srt_InlinePrintEnd,
    Srt_InlineConditionalPrintEnd,
    Srt_InlineConditionalScriptEnd,
    Srt_InlineScriptEnd,
    Srt_ScriptEnd,
    SrtEndJsEnd,
    SrtEnd,
    JsBegin,
    Js_AttributeConditionalPrintScript,
    Js_InlineConditionalPrintScript,
    Js_InlineConditionalScript,
    Js_AttributePrintScript,
    Js_InlinePrintScript,
    Js_InlineScript,
    Js_Script,
    JsEnd,
    JsCCommentBegin,
    JsCC_Begin1,
    JsCC_Begin2,
    JsCC_Begin3,
    JsCC_Begin4,
    JsCC_Begin5,
    JsCC_Begin6,
    JsCC_Begin7,
    JsCC_Commnent1,
    JsCC_Commnent2,
    JsCC_Commnent3,
    JsCC_Commnent4,
    JsCC_Commnent5,
    JsCC_Commnent6,
    JsCC_Commnent7,
    JsCC_End1,
    JsCC_End2,
    JsCC_End3,
    JsCC_End4,
    JsCC_End5,
    JsCC_End6,
    JsCC_End7,
    JsCCommentEnd,
    JsCommentBegin,
    JsC_Begin1,
    JsC_Begin2,
    JsC_Begin3,
    JsC_Begin4,
    JsC_Begin5,
    JsC_Begin6,
    JsC_Begin7,
    JsC_Commnent1,
    JsC_Commnent2,
    JsC_Commnent3,
    JsC_Commnent4,
    JsC_Commnent5,
    JsC_Commnent6,
    JsC_Commnent7,
    JsC_End1,
    JsC_End2,
    JsC_End3,
    JsC_End4,
    JsC_End5,
    JsC_End6,
    JsC_End7,
    JsCommentEnd,
};
*/

#endif // SYNTAXHIGHLIGHTER_H
