/*
    Aseba - an event-based framework for distributed robot control
    Created by Stéphane Magnenat <stephane at magnenat dot net> (http://stephane.magnenat.net)
    with contributions from the community.
    Copyright (C) 2007--2018 the authors, see authors.txt for details.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef THYMIO_VISUAL_PROGRAMMING_H
#define THYMIO_VISUAL_PROGRAMMING_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QGraphicsItem>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QComboBox>
#include <QToolBar>
#include <QPushButton>
#include <QDomDocument>
#include <QStack>
#include <QLabel>

#include <map>
#include <vector>
#include <iterator>
#include <memory>

namespace Aseba {
struct ThymioVPLStandaloneInterface;
class ThymioVPLStandalone;
}  // namespace Aseba

namespace Aseba {
namespace ThymioVPL {
    class Scene;
    class BlockButton;
    class ResizingView;

    /** \addtogroup studio */
    /*@{*/

    class ThymioVisualProgramming : public QWidget {
        Q_OBJECT

    public:
        ThymioVisualProgramming();
        ~ThymioVisualProgramming() override;

        QWidget* createMenuEntry();
        void closeAsSoonAsPossible();

        void aboutToLoad();
        void loadFromDom(const QDomDocument& content, bool fromFile);
        virtual QDomDocument transformDomToVersion1(const QDomDocument& content0);
        QDomDocument saveToDom() const;
        void codeChangedInEditor();

        bool isModified() const;
        qreal getViewScale() const;

    signals:
        void modifiedStatusChanged(bool modified);
        void compilationOutcome(bool success);

    protected slots:
        void showErrorLine();
        bool closeFile();

    private slots:
        void openHelp();
        void saveSnapshot() const;
        void showVPLModal();

        void addEvent();
        void addAction();

        void newFile();
        void openFile();
        bool save();
        bool saveAs();
        void setColorScheme(int index);
        void run();
        void stop();
        void toggleAdvancedMode();
        void pushUndo();
        void undo();
        void redo();
        void processCompilationResult();
        void processHighlightChange();
        // void userEvent(unsigned id, const VariablesDataVector& data);

    private:
        void clearUndo();
        void toggleAdvancedMode(bool advanced, bool force = false, bool ignoreSceneCheck = false);
        void clearSceneWithoutRecompilation();
        void showAtSavedPosition();
        void setupGlobalEvents();

    public:
        bool debugLog;
        bool execFeedback;

    protected:
        friend class Aseba::ThymioVPL::BlockButton;
        friend class Aseba::ThymioVPL::Scene;

        // std::unique_ptr<DevelopmentEnvironmentInterface> de;
        ResizingView* view;
        Scene* scene;
        bool loading;  //!< true during load, to prevent recursion of changes triggered by VPL
                       //!< itself

        QStack<QString> undoStack;  //!< keep string version of QDomDocument
        int undoPos;                //!< current position of undo in the stack, -1 if invalid

        // Event & Action buttons
        QList<BlockButton*> eventButtons;
        QList<BlockButton*> actionButtons;
        QLabel* eventsLabel;
        QLabel* actionsLabel;

        QLabel* compilationResult;
        QLabel* compilationResultImage;
        QPushButton* showCompilationError;

        // QSlider *zoomSlider;

        QToolBar* toolBar;
        QGridLayout* toolLayout;
        QPushButton* newButton;
        QPushButton* openButton;
        QPushButton* saveButton;
        QPushButton* saveAsButton;
        QPushButton* undoButton;
        QPushButton* redoButton;
        QPushButton* runButton;
        QPushButton* stopButton;
        QPushButton* advancedButton;
        QPushButton* helpButton;
        QPushButton* snapshotButton;
        QFrame* firstSeparator;
        QFrame* secondSeparator;
        QSpacerItem* spacer1;
        QSpacerItem* spacer2;
        QSpacerItem* spacer3;
        QSpacerItem* spacerRunStop;
        QSpacerItem* spacer4;
        QSpacerItem* spacer5;
        QSpacerItem* spacer6;

        // run button animation
        QVector<QPixmap> runAnimFrames;
        int runAnimFrame;
        int runAnimTimer;

        QVBoxLayout* mainLayout;
        QHBoxLayout* horizontalLayout;
        QVBoxLayout* eventsLayout;
        QVBoxLayout* sceneLayout;
        QHBoxLayout* compilationResultLayout;
        QVBoxLayout* actionsLayout;

    protected:
        friend class Aseba::ThymioVPLStandalone;
        friend struct Aseba::ThymioVPLStandaloneInterface;

        QPixmap drawColorScheme(const QColor& eventColor, const QColor& stateColor, const QColor& actionColor);
        void saveGeometryIfVisible();
        bool preDiscardWarningDialog(bool keepCode);
        void clearHighlighting(bool keepCode);
        void setColors(QComboBox* comboBox);
        void updateBlockButtonImages();
        void closeEvent(QCloseEvent* event) override;

#ifndef Q_OS_WIN
        void regenerateRunButtonAnimation(const QSize& iconSize);
#endif  // Q_OS_WIN
        void startRunButtonAnimationTimer();
        void stopRunButtonAnimationTimer();
        float computeScale(QResizeEvent* event, int desiredToolbarIconSize);
        void resizeEvent(QResizeEvent* event) override;
        void timerEvent(QTimerEvent* event) override;
    };
    /*@}*/
}  // namespace ThymioVPL
}  // namespace Aseba

#endif