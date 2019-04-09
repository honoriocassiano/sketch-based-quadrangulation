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
            fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(),
                    file, context.line, function);
            break;
        case QtInfoMsg:
            fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(),
                    file, context.line, function);
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(),
                    file, context.line, function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(),
                    file, context.line, function);
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(),
                    file, context.line, function);
            break;
        }
    }
}

inline QDebug Debug() { return qDebug(); }

#else

#include <iostream>

inline std::ostream &Debug() {

    std::cout.clear();

    if (!DebugState.debug) {
        std::cout.setstate(std::ios_base::badbit);
    }

    return std::cout;
}

#endif

#endif // DEBUG_H
