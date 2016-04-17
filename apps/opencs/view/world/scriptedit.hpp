#ifndef SCRIPTEDIT_H
#define SCRIPTEDIT_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QVector>
#include <QTimer>
#include <QFont>

#include "../../model/world/universalid.hpp"

#include "scripthighlighter.hpp"

class QRegExp;

namespace CSMDoc
{
    class Document;
}

namespace CSVWorld
{
    class LineNumberArea;

    /// \brief Editor for scripts.
    class ScriptEdit : public QPlainTextEdit
    {
            Q_OBJECT

        public:

            class ChangeLock
            {
                    ScriptEdit& mEdit;

                    ChangeLock (const ChangeLock&);
                    ChangeLock& operator= (const ChangeLock&);

                public:

                    ChangeLock (ScriptEdit& edit);
                    ~ChangeLock();
            };

            friend class ChangeLock;

        private:

            int mChangeLocked;
            ScriptHighlighter *mHighlighter;
            QTimer mUpdateTimer;
            bool mShowLineNum;
            LineNumberArea *mLineNumberArea;
            QFont mDefaultFont;
            QFont mMonoFont;
            int mTabCharCount;

        protected:

            bool event (QEvent *event);

        public:

            ScriptEdit (const CSMDoc::Document& document, ScriptHighlighter::Mode mode,
                QWidget* parent);

            /// Should changes to the data be ignored (i.e. not cause updated)?
            ///
            /// \note This mechanism is used to avoid infinite update recursions
            bool isChangeLocked() const;

            void lineNumberAreaPaintEvent(QPaintEvent *event);
            int lineNumberAreaWidth();
            void showLineNum(bool show);
            void setMonoFont(bool show);

        protected:

            virtual void resizeEvent(QResizeEvent *e);

        private:

            QVector<CSMWorld::UniversalId::Type> mAllowedTypes;
            const CSMDoc::Document& mDocument;
            const QRegExp mWhiteListQoutes;

            void dragEnterEvent (QDragEnterEvent* event);

            void dropEvent (QDropEvent* event);

            void dragMoveEvent (QDragMoveEvent* event);

            bool stringNeedsQuote(const std::string& id) const;

            /// \brief Set tab width for script editor.
            void setTabWidth();

            /// \brief Turn line wrapping in script editor on or off.
            /// \param wrap Whether or not to wrap lines.
            void wrapLines(bool wrap);

        private slots:

            /// \brief Update editor when related setting is changed.
            /// \param setting Setting that was changed.
            void settingChanged(const CSMPrefs::Setting *setting);

            void idListChanged();

            void updateHighlighting();

            void updateLineNumberAreaWidth(int newBlockCount);

            void updateLineNumberArea(const QRect &, int);
    };

    class LineNumberArea : public QWidget
    {
            ScriptEdit *mScriptEdit;

        public:

            LineNumberArea(ScriptEdit *editor);
            QSize sizeHint() const;

        protected:

            void paintEvent(QPaintEvent *event);
    };
}
#endif // SCRIPTEDIT_H
