//
// Created by ronyshchuk on 14.04.20.
//

#ifndef LEXI_PAGE_HPP
#define LEXI_PAGE_HPP

#include "i_composite_glyph.hpp"

class TextView;
class Row;

class Page: public ICompositeGlyph
{
    using RowPtr = std::shared_ptr<Row>;

public:
    explicit Page(TextView* parent, const GlyphParams& params);

    void Draw(Gui::Window* window) override;
    void ProcessEvent(Gui::Window* window, const Event& event) override;

    void DrawCursor(Gui::Window* window);

    [[nodiscard]] bool IsBottomRow(const GlyphPtr&) const;

    std::shared_ptr<Row> GetFirstRow();
    void SetCurrentRow(std::shared_ptr<Row> row){m_currentRow = std::move(row);}

    [[nodiscard]] bool IsEmpty() const { return m_components.empty();}
    [[nodiscard]] bool IsFull() const;

    /**
     * Check whether a new row can be added to the current Page.
     * @todo remplace IsFull with this.
     * @param height Height of the new row.
     * @return True if new row fits to the page params.
     */
    [[nodiscard]] bool RowCanBeAdded(height_t height) const;
    std::shared_ptr<Row> RemoveFirstRow();

    /**
     * Insert @p row in the beginning of the page
     * @note All glyphs after @p position will be moved down accordingly.
     * @param row Row to insert.
     * @param position Position to insert.
     */
    void Insert(GlyphPtr glyph, size_t position) override;

private:
    void ProcessBackspaceFromBeginning(Gui::Window*);
    void ProcessCharacterShift(std::shared_ptr<Row>&, IGlyph::GlyphPtr&);

    /**
     * For each row in page, starting from @p startRowIt, updates their position to previous row.
     * @param startRowIt Row from which begin shift/
     */
    void MoveUpLowerRows(Gui::Window* w, std::list<IGlyph::GlyphPtr>::iterator startRowIt);

    std::shared_ptr<Row> GetPreviousRow();
    std::shared_ptr<Row> GetPreviousRow(IGlyph::GlyphPtr &row);
    std::shared_ptr<Row> GetNextRow();
    std::shared_ptr<Row> GetNextRow(IGlyph::GlyphPtr &row);
    /**
     * @return Pointer to the last row.
     */
    RowPtr GetLastRow();

    // text view indentations
    static height_t m_topIndent;
    static height_t m_botIndent;
    static width_t m_leftIndent;
    static width_t m_rightIndent;

    std::shared_ptr<Row> m_currentRow;
    TextView* m_parent;
};

// height_t Page::m_topIndent = 25;
// height_t Page::m_botIndent = 75;
// width_t Page::m_leftIndent = 50;
// width_t Page::m_rightIndent = 50;

#endif  // LEXI_PAGE_HPP
