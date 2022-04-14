#pragma once

#include <QtGui/QSyntaxHighlighter>
#include <QtCore/QRegularExpression>


// https://github.com/noct/sublime-shaders
class cgSyntaxHighlighter : QSyntaxHighlighter
{
    //Q_OBJECT;
public:
    cgSyntaxHighlighter(QTextDocument* doc) : QSyntaxHighlighter(doc)
    {
        this->std_PS_declaration = QRegularExpression(QStringLiteral("^void std_PS\\s*\\(.*\\)"));
        this->std_VS_declaration = QRegularExpression(QStringLiteral("^void std_VS\\s*\\(.*\\)"));

        HighlightingRule rule;

        // Variablities
        variabilitiesFormat.setForeground(Qt::blue);
        const QString variabilitiesPatterns[] = {
            QStringLiteral("\\bconst\\b"), QStringLiteral("\\bextern\\b"), QStringLiteral("\\bin\\b"),
            QStringLiteral("\\binline\\b"), QStringLiteral("\\binout\\b"), QStringLiteral("\\bstatic\\b"),
            QStringLiteral("\\bout\\b"), QStringLiteral("\\buniform\\b"), QStringLiteral("\\bvarying\\b")
        };
        for (const QString& pattern : variabilitiesPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = variabilitiesFormat;
            highlightingRules.append(rule);
        }




        // Control Flow Statements
        controlFlowFormat.setForeground(Qt::magenta);
        const QString controlFlow[] = {
            QStringLiteral("\\bbreak\\b"), QStringLiteral("\\bcontinue\\b"), QStringLiteral("\\bdiscard\\b"),
            QStringLiteral("\\bdo\\b"), QStringLiteral("\\bfor\\b"), QStringLiteral("\\bif\\b"),
            QStringLiteral("\\breturn\\b"), QStringLiteral("\\bswitch\\b"), QStringLiteral("\\bwhile\\b")
        };
        for (const QString& pattern : controlFlow) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = controlFlowFormat;
            highlightingRules.append(rule);
        }

        // Numbers
        numericFormat.setForeground(QBrush({ 50, 255, 50 }));
        rule.pattern = QRegularExpression(QStringLiteral("\\b([0-9]+\\.?[0-9]*)|(\\.[0-9]+)\\b"));
        rule.format = numericFormat;
        highlightingRules.append(rule);

        // Quotes
        quotationFormat.setForeground(QBrush({ 255, 255, 0 }));
        rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        // Functions
        functionFormat.setForeground(Qt::darkYellow);
        rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+\\s*(?=\\()"));
        rule.format = functionFormat;
        highlightingRules.append(rule);

        // Intrinsic Function Types
        intrinsicFunctionFormat.setForeground(Qt::darkCyan);
        const QString intrinsicFunctionPatterns[] = {
            QStringLiteral("\\babs"), QStringLiteral("\\bacos"), QStringLiteral("\\ball"), QStringLiteral("\\bany"),
            QStringLiteral("\\basin"), QStringLiteral("\\batan"), QStringLiteral("\\batan2"), QStringLiteral("\\bbitCount"),
            QStringLiteral("\\bbitfieldExtract"), QStringLiteral("\\bbitfieldInsert"), QStringLiteral("\\bbitfieldReverse"), QStringLiteral("\\bceil"),
            QStringLiteral("\\bclamp"), QStringLiteral("\\clip"), QStringLiteral("\\bcos"), QStringLiteral("\\bcosh"),
            QStringLiteral("\\bcross"), QStringLiteral("\\bddx"), QStringLiteral("\\bddy"), QStringLiteral("\\bdegrees"),
            QStringLiteral("\\bdeterminant"), QStringLiteral("\\bdistance"), QStringLiteral("\\bdot"), QStringLiteral("\\bexp"),
            QStringLiteral("\\bexp2"), QStringLiteral("\\bfaceforward"), QStringLiteral("\\bfindLSB"), QStringLiteral("\\bfindMSB"),
            QStringLiteral("\\bfloatToIntBits"), QStringLiteral("\\bfloatToRawIntBits"), QStringLiteral("\\bfloor"), QStringLiteral("\\bfmod"),
            QStringLiteral("\\bfrac"), QStringLiteral("\\bfrexp"), QStringLiteral("\\bfwidth"), QStringLiteral("\\bintBitsToFloat"),
            QStringLiteral("\\binverse"), QStringLiteral("\\bisfinite"), QStringLiteral("\\bisinf"), QStringLiteral("\\bisnan"),
            QStringLiteral("\\bldexp"), QStringLiteral("\\blength"), QStringLiteral("\\blerp"), QStringLiteral("\\blit"),
            QStringLiteral("\\blog"), QStringLiteral("\\blog10"), QStringLiteral("\\blog2"), QStringLiteral("\\bmax"),
            QStringLiteral("\\bmin"), QStringLiteral("\\bmodf"), QStringLiteral("\\bmul"), QStringLiteral("\\bnormalize"),
            QStringLiteral("\\bpow"), QStringLiteral("\\bradians"), QStringLiteral("\\breflect"), QStringLiteral("\\brefract"),
            QStringLiteral("\\bround"), QStringLiteral("\\brsqrt"), QStringLiteral("\\bsaturate"), QStringLiteral("\\bsign"),
            QStringLiteral("\\bsin"), QStringLiteral("\\bsincos"), QStringLiteral("\\bsinh"), QStringLiteral("\\bsmoothstep"),
            QStringLiteral("\\bsqrt"), QStringLiteral("\\bstep"), QStringLiteral("\\btan"), QStringLiteral("\\btanh"),
            QStringLiteral("\\btex1D"), QStringLiteral("\\btex1Dbias"), QStringLiteral("\\btex1Dcmpbias"), QStringLiteral("\\btex1Dcmpbias"),
            QStringLiteral("\\btex1Dcmplod"), QStringLiteral("\\btex1Dfetch"), QStringLiteral("\\btex1Dlod"), QStringLiteral("\\btex1Dproj"),
            QStringLiteral("\\btex1Dsize"), QStringLiteral("\\btex1DARRAY"), QStringLiteral("\\btex1DARRAYbias"), QStringLiteral("\\btex1DARRAYcmpbias"),
            QStringLiteral("\\btex1DARRAYcmplod"), QStringLiteral("\\btex1DARRAYfetch"), QStringLiteral("\\btex1DARRAYlod"), QStringLiteral("\\btex1DARRAYproj"),
            QStringLiteral("\\btex1DARRAYsize"), QStringLiteral("\\btex1DARRAYbias"), QStringLiteral("\\btex2D"), QStringLiteral("\\btex2Dbias"),
            QStringLiteral("\\btex2Dcmpbias"), QStringLiteral("\\btex2Dcmplod"), QStringLiteral("\\btex2Dfetch"), QStringLiteral("\\btex2Dlod"),
            QStringLiteral("\\btex2Dproj"), QStringLiteral("\\btex2Dsize"), QStringLiteral("\\btex2DARRAY"), QStringLiteral("\\btex2DARRAYbias"),
            QStringLiteral("\\btex2DARRAYcmpbias"), QStringLiteral("\\btex2DARRAYcmplod"), QStringLiteral("\\btex2DARRAYfetch"), QStringLiteral("\\btex2DARRAYlod"),
            QStringLiteral("\\btex2DARRAYproj"), QStringLiteral("\\btex2DARRAYsize"), QStringLiteral("\\btex2DARRAYbias"), QStringLiteral("\\btex2DMSfetch"),
            QStringLiteral("\\btex2DMSARRAYfetch"), QStringLiteral("\\btex2DMSARRAYsize"), QStringLiteral("\\btex3D"), QStringLiteral("\\btex3Dbias"),
            QStringLiteral("\\btex3Dcmpbias"), QStringLiteral("\\btex3Dcmplod"), QStringLiteral("\\btex3Dfetch"), QStringLiteral("\\btex3Dlod"),
            QStringLiteral("\\btex3Dproj"), QStringLiteral("\\btex3Dsize"), QStringLiteral("\\btexBUF"), QStringLiteral("\\btexBUFsize"),
            QStringLiteral("\\btexCUBE"), QStringLiteral("\\btexCUBEARRAY"), QStringLiteral("\\btexCUBEARRAYbias"), QStringLiteral("\\btexCUBEARRAYlod"),
            QStringLiteral("\\btexCUBEARRAYsize"), QStringLiteral("\\btexCUBEbias"), QStringLiteral("\\btexCUBElod"), QStringLiteral("\\btexCUBEproj"),
            QStringLiteral("\\btexCUBEsize"), QStringLiteral("\\btexRBUF"), QStringLiteral("\\btexRBUFsize"), QStringLiteral("\\btexRECT"),
            QStringLiteral("\\btexRECTbias"), QStringLiteral("\\btexRECTfetch"), QStringLiteral("\\btexRECTlod"), QStringLiteral("\\btexRECTproj"),
            QStringLiteral("\\btexRECTsize"), QStringLiteral("\\btranspose"), QStringLiteral("\\btrunc")

        };
        for (const QString& pattern : intrinsicFunctionPatterns) {
            rule.pattern = QRegularExpression(pattern + QStringLiteral("\\s*(?=\\()"));
            rule.format = intrinsicFunctionFormat;
            highlightingRules.append(rule);
        }

        // Basic Types
        basicTypesFormat.setForeground(Qt::blue);
        const QString basicTypesPatterns[] = {
            QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bbool\\b"), QStringLiteral("\\bstruct\\b"),
            QStringLiteral("\\bfloat\\b"), QStringLiteral("\\bhalf\\b"), QStringLiteral("\\bfixed\\b"),
            QStringLiteral("\\bchar\\b"), QStringLiteral("\\bshort\\b"), QStringLiteral("\\blong\\b"),
            QStringLiteral("\\bint\\b"), QStringLiteral("\\bunsigned\\b")
        };
        for (const QString& pattern : basicTypesPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = basicTypesFormat;
            highlightingRules.append(rule);
        }

        // Vector Types
        vectorTypesFormat.setForeground(Qt::blue);
        const QString vectorTypesPatterns[] = {
            QStringLiteral("\\bfloat1\\b"), QStringLiteral("\\bfloat2\\b"), QStringLiteral("\\bfloat3\\b"), QStringLiteral("\\bfloat4\\b"),
            QStringLiteral("\\bhalf1\\b"), QStringLiteral("\\bhalf2\\b"), QStringLiteral("\\bhalf3\\b"), QStringLiteral("\\bhalf4\\b"),
            QStringLiteral("\\bfixed1\\b"), QStringLiteral("\\bfixed2\\b"), QStringLiteral("\\bfixed3\\b"), QStringLiteral("\\bfixed4\\b"),
            QStringLiteral("\\bint1\\b"), QStringLiteral("\\bint2\\b"), QStringLiteral("\\bint3\\b"), QStringLiteral("\\bint4\\b")
        };
        for (const QString& pattern : vectorTypesPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = vectorTypesFormat;
            highlightingRules.append(rule);
        }

        // Matrix Types
        matrixTypesFormat.setForeground(Qt::blue);
        const QString matrixTypesPatterns[] = {
            QStringLiteral("\\bfloat1x1\\b"), QStringLiteral("\\bfloat1x2\\b"), QStringLiteral("\\bfloat1x3\\b"), QStringLiteral("\\bfloat1x4\\b"),
            QStringLiteral("\\bfloat2x1\\b"), QStringLiteral("\\bfloat2x2\\b"), QStringLiteral("\\bfloat2x3\\b"), QStringLiteral("\\bfloat2x4\\b"),
            QStringLiteral("\\bfloat3x1\\b"), QStringLiteral("\\bfloat3x2\\b"), QStringLiteral("\\bfloat3x3\\b"), QStringLiteral("\\bfloat3x4\\b"),
            QStringLiteral("\\bfloat4x1\\b"), QStringLiteral("\\bfloat4x2\\b"), QStringLiteral("\\bfloat4x3\\b"), QStringLiteral("\\bfloat4x4\\b"),
            QStringLiteral("\\bhalf1x1\\b"), QStringLiteral("\\bhalf1x2\\b"), QStringLiteral("\\bhalf1x3\\b"), QStringLiteral("\\bhalf1x4\\b"),
            QStringLiteral("\\bhalf2x1\\b"), QStringLiteral("\\bhalf2x2\\b"), QStringLiteral("\\bhalf2x3\\b"), QStringLiteral("\\bhalf2x4\\b"),
            QStringLiteral("\\bhalf3x1\\b"), QStringLiteral("\\bhalf3x2\\b"), QStringLiteral("\\bhalf3x3\\b"), QStringLiteral("\\bhalf3x4\\b"),
            QStringLiteral("\\bhalf4x1\\b"), QStringLiteral("\\bhalf4x2\\b"), QStringLiteral("\\bhalf4x3\\b"), QStringLiteral("\\bhalf4x4\\b"),
            QStringLiteral("\\bfixed1x1\\b"), QStringLiteral("\\bfixed1x2\\b"), QStringLiteral("\\bfixed1x3\\b"), QStringLiteral("\\bfixed1x4\\b"),
            QStringLiteral("\\bfixed2x1\\b"), QStringLiteral("\\bfixed2x2\\b"), QStringLiteral("\\bfixed2x3\\b"), QStringLiteral("\\bfixed2x4\\b"),
            QStringLiteral("\\bfixed3x1\\b"), QStringLiteral("\\bfixed3x2\\b"), QStringLiteral("\\bfixed3x3\\b"), QStringLiteral("\\bfixed3x4\\b"),
            QStringLiteral("\\bfixed4x1\\b"), QStringLiteral("\\bfixed4x2\\b"), QStringLiteral("\\bfixed4x3\\b"), QStringLiteral("\\bfixed4x4\\b"),
            QStringLiteral("\\bint1x1\\b"), QStringLiteral("\\bint1x2\\b"), QStringLiteral("\\bint1x3\\b"), QStringLiteral("\\bint1x4\\b"),
            QStringLiteral("\\bint2x1\\b"), QStringLiteral("\\bint2x2\\b"), QStringLiteral("\\bint2x3\\b"), QStringLiteral("\\bint2x4\\b"),
            QStringLiteral("\\bint3x1\\b"), QStringLiteral("\\bint3x2\\b"), QStringLiteral("\\bint3x3\\b"), QStringLiteral("\\bint3x4\\b"),
            QStringLiteral("\\bint4x1\\b"), QStringLiteral("\\bint4x2\\b"), QStringLiteral("\\bint4x3\\b"), QStringLiteral("\\bint4x4\\b"),
        };
        for (const QString& pattern : matrixTypesPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = matrixTypesFormat;
            highlightingRules.append(rule);
        }

        // Sampler Types
        samplerTypesFormat.setForeground(Qt::blue);
        const QString samplerTypesPatterns[] = {
            QStringLiteral("\\bsampler\\b"), QStringLiteral("\\bsampler1D\\b"), QStringLiteral("\\bsampler2D\\b"), QStringLiteral("\\bsampler3D\\b"),
            QStringLiteral("\\bsamplerCUBE\\b"), QStringLiteral("\\bsamplerRECT\\b")
        };
        for (const QString& pattern : samplerTypesPatterns) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = samplerTypesFormat;
            highlightingRules.append(rule);
        }


        // Input Characters
        inputVariableFormat.setForeground(Qt::darkGray);

        // Comments
        singleLineCommentFormat.setForeground(Qt::darkGreen);
        rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::darkGreen);

        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
    };
protected:
    void highlightBlock(const QString& text)
    {
        QRegularExpressionMatch ps_match = this->std_PS_declaration.match(text);
        QRegularExpressionMatch vs_match = this->std_VS_declaration.match(text);
        QString match_text;
        bool found_declaration = true;
        if (ps_match.hasMatch())
            match_text = ps_match.captured();
        else if (vs_match.hasMatch())
            match_text = vs_match.captured();
        else
            found_declaration = false;

        if (found_declaration)
        {
            // Inputs with attributes
            QRegularExpressionMatchIterator matchIterator = QRegularExpression(QStringLiteral("(?=\\s*)([a-zA-Z0-9_]+)(?=\\s*:)")).globalMatch(match_text);
            HighlightingRule rule;
            this->inputVariableRules.clear();
            while (matchIterator.hasNext())
            {
                QRegularExpressionMatch match = matchIterator.next();
                auto msg = QString::fromStdString("\\b" + match.captured(0).toStdString() + "(?=[^a-zA-Z0-9])");
                rule.pattern = QRegularExpression(msg);
                rule.format = inputVariableFormat;
                this->inputVariableRules.append(rule);
            }

            // Inputs without attributes
            matchIterator = QRegularExpression(QStringLiteral("(?:[^:]\\s+)([a-zA-Z0-9_]+)(?=\\s*,|\\s*\\))")).globalMatch(match_text);
            while (matchIterator.hasNext())
            {
                QRegularExpressionMatch match = matchIterator.next();
                auto msg = QString::fromStdString("\\b" + match.captured(1).toStdString() + "(?=[^a-zA-Z0-9])");
                rule.pattern = QRegularExpression(msg);
                rule.format = inputVariableFormat;
                this->inputVariableRules.append(rule);
            }
        }

        for (const HighlightingRule& rule : qAsConst(highlightingRules)) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }


        for (const HighlightingRule& rule : qAsConst(inputVariableRules)) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }

        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = text.indexOf(commentStartExpression);

        while (startIndex >= 0) {
            QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            }
            else {
                commentLength = endIndex - startIndex
                    + match.capturedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
    }
private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
    QVector<HighlightingRule> inputVariableRules;

    QRegularExpression std_PS_declaration;
    QRegularExpression std_VS_declaration;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;


    QTextCharFormat inputVariableFormat;

    QTextCharFormat variabilitiesFormat;
    QTextCharFormat basicTypesFormat;
    QTextCharFormat vectorTypesFormat;
    QTextCharFormat matrixTypesFormat;
    QTextCharFormat samplerTypesFormat;
    QTextCharFormat controlFlowFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat intrinsicFunctionFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat numericFormat;
};

