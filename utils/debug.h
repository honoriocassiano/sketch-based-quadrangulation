#ifndef DEBUG_H
#define DEBUG_H

struct _ds {
    bool debug;
};

static _ds DebugState{true};

#ifdef QT_VERSION

#include <qapplication.h>
#include <qdebug.h>

static void myMessageOutput(QtMsgType type, const QMessageLogContext &context,
                            const QString &msg) {

    if (DebugState.debug) {
        QByteArray localMsg = msg.toLocal8Bit();
        const char *file = context.file ? context.file : "";
        const char *function = context.function ? context.function : "";
        switch (type) {
        case QtDebugMsg:
            fprintf(stderr, "%s (%s:%u)\n", localMsg.constData(), file,
                    context.line);
            break;
        case QtInfoMsg:
            fprintf(stderr, "Info: %s (%s:%u)\n", localMsg.constData(), file,
                    context.line);
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s (%s:%u)\n", localMsg.constData(), file,
                    context.line);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s (%s:%u)\n", localMsg.constData(),
                    file, context.line);
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s (%s:%u)\n", localMsg.constData(), file,
                    context.line);
            break;
        }
    }
}

#define Debug() qDebug()

#else

#include <iostream>

inline std::ostream &_debug() {

    std::cout.clear();

    if (!DebugState.debug) {
        std::cout.setstate(std::ios_base::badbit);
    }

    return std::cout;
}

/// Debugger if not using Qt
#define Debug() _debug()

#endif

#endif // DEBUG_H
